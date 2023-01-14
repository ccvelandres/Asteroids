#include <core/audio/audioManager.hpp>
#include <core/utils/logging.hpp>
#include <core/utils/memory.hpp>
#include <core/ecs/components/audioComponent.hpp>
#include <audio/audio_p.hpp>
#include <audio/audioManager_p.hpp>

#include <SDL.h>
#include <SDL_sound.h>

#include <AL/al.h>
#include <AL/alc.h>

#include <vector>
#include <algorithm>
#include <unordered_map>

/**
 * @file: Audio manager
 * @todo: possibly extend audio mixing with OpenAL
 * @todo: this file needs cleaning up
 * @todo: on demand decoding? SDL Mixer for SEs decodes audio all at once
 * @todo: resource protection
 */

namespace core::audio
{
    constexpr int audioDeviceAllowedChanges = SDL_AUDIO_ALLOW_ANY_CHANGE;
    constexpr int defaultMixChannelCount    = CONFIG_CORE_AUDIO_MIX_CHANNELS;

    /** @todo: is it worth moving these private static vars as unique_ptr<Internal> to
     * AudioManager?
     */

    struct AudioDevice
    {
        SDL_AudioDeviceID deviceId;
        SDL_AudioSpec     spec;
    };

    static const char* m_alDeviceSpecifier;
    static ALCdevice  *m_alDevice;
    static ALCcontext *m_alContext;
    static uint32_t    m_alAudioFrequency;

    static AudioDevice     m_audioDevice;
    static SDL_AudioFormat m_audioFormat    = AUDIO_S16LSB;
    static uint8_t         m_audioChannels  = 2;
    static uint32_t        m_audioFrequency = 44100;

    static Sound_AudioInfo                                           m_decodeInfo;
    static std::weak_ptr<AudioListener>                              m_audioListener;
    static std::vector<std::weak_ptr<Audio>>                         m_audioClips;
    static std::unordered_map<AssetName, std::shared_ptr<AudioData>> m_audioDataCache;
    static int                                                       m_audioMixChannelCount;

    static inline uint32_t getAudioDeviceFrequency(ALCdevice *device)
    {
        L_TAG("AudioManager::getAudioDeviceFrequency");
        ALCenum err;
        ALCint  alcFrequency = 0;
        alcGetIntegerv(device, ALC_FREQUENCY, 1, &alcFrequency);
        if ((err = alcGetError(device)) != AL_NO_ERROR)
        {
            L_ERROR("Failed to query audio device frequency");
        }
        return alcFrequency;
    }

    AudioManager::AudioManager() { L_TAG("AudioManager::AudioManager"); }
    AudioManager::~AudioManager() { L_TAG("AudioManager::!AudioManager"); }

    bool AudioManager::init()
    {
        L_TAG("AudioManager::init");

        ALenum err;

        // Verify SDL Audio was initialized
        if (!(SDL_WasInit(SDL_INIT_AUDIO) & SDL_INIT_AUDIO)) L_THROW_RUNTIME("SDL_INIT_AUDIO was not initialized");

        /** @todo: in SDL 2.24.0, use SDL_GetDefaultAudioInfo to query default audio device */

        // Enumerate audio devices
        ALboolean enumPresent = alcIsExtensionPresent(NULL, "ALC_ENUMERATION_EXT");
        if (enumPresent)
        {
            const ALchar *devName = alcGetString(NULL, ALC_DEVICE_SPECIFIER);
            L_INFO("Found audio device(s):");
            while (devName != NULL && *devName != '\0')
            {
                L_INFO("\t{}", devName);
                while (true)
                {
                    if (*(devName++) == '\0' && *(devName) == '\0')
                    {
                        break;
                    }
                }
            }
        }
        else
        {
            L_INFO("Could not enumerate audio devices.");
        }

        {
            PROFILER_BLOCK("AudioManager::init::initAL");

            // Open dfault audio device
            m_alDevice = alcOpenDevice(NULL);
            if (!m_alDevice)
            {
                L_THROW_RUNTIME("Could not open audio playback device");
            }

            // Query audio device specifier
            m_alDeviceSpecifier = alcGetString(m_alDevice, ALC_DEVICE_SPECIFIER);
            if((err = alGetError()) != AL_NO_ERROR)
            {
                L_ERROR("Could not query audio device specifier");
            }
            L_INFO("Default audio device: {}", m_alDeviceSpecifier);

            // Create al context
            m_alContext = alcCreateContext(m_alDevice, NULL);
            if (!alcMakeContextCurrent(m_alContext))
            {
                L_THROW_RUNTIME("Failed to switch context current");
            }

            // Query context attributes
            m_alAudioFrequency = getAudioDeviceFrequency(m_alDevice);
        }

        {
            PROFILER_BLOCK("AudioManager::init::initDecoders");

            // Initialize SDL2_Sound used for audio decoding
            if (!Sound_Init())
            {
                L_THROW_RUNTIME("Could not initialize SDL_Sound");
            }
            else
            {
                Sound_Version compiled, linked;

                SOUND_VERSION(&compiled);
                Sound_GetLinkedVersion(&linked);

                L_INFO("SDL2_Sound compiled with: {}.{}.{}", compiled.major, compiled.minor, compiled.patch);
                L_INFO("Using linked SDL2_Sound: {}.{}.{}", linked.major, linked.minor, linked.patch);
            }

            // Get list of available decoders
            for (const Sound_DecoderInfo **decoders = Sound_AvailableDecoders(); *decoders != NULL; decoders++)
            {
                L_INFO("Found decoder: {}", (*decoders)->description);
            }

            // // Setup requested raw audio stream format
            m_decodeInfo = {.format = m_audioFormat, .channels = m_audioChannels, .rate = m_audioFrequency};
        }

        return true;
    }

    void AudioManager::cleanup() { L_TAG("AudioManager::cleanup"); }
    void AudioManager::preUpdate() {}
    void AudioManager::fixedUpdate(const time_ms &delta) {}
    void AudioManager::update(const time_ms &delta) {}
    void AudioManager::postUpdate() {}
    void AudioManager::refresh()
    {
        L_TAG("AudioManager::refresh");
        std::remove_if(m_audioClips.begin(), m_audioClips.end(), [](std::weak_ptr<Audio> &c) {
            return c.expired() ? true : false;
        });
    }

    void AudioManager::setListener(const ComponentPtr<AudioListener> &audioListener)
    {
        L_TAG("AudioManager::setListener");
        m_audioListener = audioListener;
    }

    std::shared_ptr<AudioData> AudioManager::loadAudioFile(const AssetName &assetName)
    {
        L_TAG("AudioManager::loadAudioFile");
        ALenum                     err;
        std::shared_ptr<AudioData> audioData;

        // Check if asset was loaded already
        auto idCache = m_audioDataCache.find(assetName);
        if (idCache != m_audioDataCache.end())
        {
            if (idCache->second)
            {
                L_DEBUG("Audio loaded from cache: {}", assetName);
                return idCache->second;
            }
        }

        // Load asset to memory
        audioData      = std::make_shared<AudioData>(AudioData());
        auto assetPath = AssetInventory::getInstance().resolvePath(AssetType::Audio, assetName);
        L_ASSERT(assetPath.size() == 1, "Found multiple asset paths for: {}", assetName);

        // Open audio stream
        UniqueSoundSample sample(Sound_NewSampleFromFile(assetPath[0].c_str(), &m_decodeInfo, 131136));
        if (!sample) L_THROW_RUNTIME("Failed to open audio file");

        // Decode audio stream
        uint32_t soundSize = Sound_DecodeAll(sample.get());
        if (sample->flags & SOUND_SAMPLEFLAG_ERROR) L_THROW_RUNTIME("Failed to decode audio file");
        if (sample->actual.format != m_decodeInfo.format)
            L_THROW_RUNTIME("Decoded format does not match desired format");

        // Conversion handled by Sound_DecodeAll (SDL_ConvertAudio underneath)
        audioData->format    = AL_FORMAT_STEREO16;
        audioData->bitDepth  = SDL_AUDIO_BITSIZE(m_decodeInfo.format);
        audioData->channels  = m_decodeInfo.channels;
        audioData->frequency = m_decodeInfo.rate;
        audioData->size      = soundSize;
        audioData->duration  = Sound_GetDuration(sample.get());

        // Load to OpenAL buffers
        alGetError();
        alGenBuffers((ALuint)1, &audioData->bufferId);
        if (err = alGetError() != AL_NO_ERROR) L_THROW_RUNTIME("Could not create buffer object");
        alBufferData(audioData->bufferId, audioData->format, sample->buffer, audioData->size, audioData->frequency);
        if (err = alGetError() != AL_NO_ERROR) L_THROW_RUNTIME("Could not copy buffer data");

        ALint frequency, bits, channels, size;
        alGetBufferi(audioData->bufferId, AL_FREQUENCY, &frequency);
        alGetBufferi(audioData->bufferId, AL_BITS, &bits);
        alGetBufferi(audioData->bufferId, AL_CHANNELS, &channels);
        alGetBufferi(audioData->bufferId, AL_SIZE, &size);

        float duration = (audioData->size * 8) / (audioData->channels * audioData->bitDepth);

        /** @todo: make sure map insert is atomic */
        m_audioDataCache.insert(std::make_pair(assetName, audioData));

        L_DEBUG("Audio loaded from file: {}", assetName);
        return audioData;
    }

    void AudioManager::registerAudio(std::shared_ptr<Audio> &audio)
    {
        L_TAG("AudioManager::registerAudio(AudioClip)");

        m_audioClips.push_back(audio);
        L_TRACE("Audio registered: {}", audio->getAssetName());
    }

    void AudioManager::createSource(Audio::Internal &audio) { ALenum err; }

    void AudioManager::playAudio(Audio &clip)
    {
        L_TAG("AudioManager::playAudio");

        // alSource3f()

        // int res = Mix_PlayChannel(-1, clip.m_internal->m_audioData->chunk.get(), clip.m_loop);
        // if (!res) L_WARN("Could not play audio: {}", clip.m_assetName);
    }

    void AudioManager::pauseAudio(Audio &clip) { L_TAG("AudioManager::pauseAudio"); }

    void AudioManager::stopAudio(Audio &clip) { L_TAG("AudioManager::stopAudio"); }

}; // namespace core::audio
