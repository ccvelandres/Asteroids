#include <core/audio/audioManager.hpp>
#include <core/utils/logging.hpp>
#include <core/utils/memory.hpp>
#include <core/ecs/components/audioComponent.hpp>
#include <audio/audioClip_p.hpp>
#include <audio/audioManager_p.hpp>

#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>

#include <vector>
#include <algorithm>
#include <unordered_map>

/**
 * @file: Audio manager
 * @todo: possibly extend audio mixing with OpenAL
 * @todo: this file needs cleaning up
 */

namespace core::audio::manager
{
    constexpr int             audioDeviceAllowedChanges = 0;
    constexpr int             defaultAudioFrequency     = 48000;
    constexpr uint8_t         defaultAudioChannel       = 2;
    constexpr uint16_t        defaultAudioSamples       = 4096;
    constexpr SDL_AudioFormat defaultAudioFormat        = AUDIO_F32SYS;

    struct AudioDevice
    {
        SDL_AudioDeviceID deviceId;
        SDL_AudioSpec     spec;
    };

    static AudioDevice                                             audioDevice;
    static std::vector<std::weak_ptr<AudioClip>>                   audioClips;
    static std::unordered_map<AssetName, std::weak_ptr<AudioData>> audioDataCache;

    static void audioCallback(void *userdata, Uint8 *stream, int len)
    {
        L_TAG("core::audio::manager::audioCallback");
        std::vector<std::shared_ptr<AudioClip>> clips;
        // Build shared_ptr vector for active clips
        std::for_each(audioClips.begin(), audioClips.end(), [&clips](std::weak_ptr<AudioClip> &c) {
            auto p = c.lock();
            if (p && p->isPlaying()) clips.push_back(p);
        });

        // Clear stream buffer
        memset(stream, 0, len);

        for (auto &clip : clips)
        {
            /** @todo: mix active waveforms then submit to sdl2 */
        }
    }

    std::shared_ptr<AudioData> loadAudioFile(const AssetName &assetName)
    {
        L_TAG("core::audio::manager::loadAudioFile");
        std::shared_ptr<AudioData> internal;

        // Check if asset was loaded already
        auto idCache = audioDataCache.find(assetName);
        if (idCache != audioDataCache.end())
        {
            internal = idCache->second.lock();
            if (internal)
            {
                L_DEBUG("Audio loaded from cache: {}", assetName);
                return internal;
            }
        }

        // Load asset to memory
        internal       = std::make_shared<AudioData>(AudioData());
        auto assetPath = AssetInventory::getInstance().resolvePath(AssetType::Audio, assetName);
        L_ASSERT(assetPath.size() == 1, "Found multiple asset paths for: {}", assetName);

        uint8_t *audioBuffer;
        if (SDL_LoadWAV(assetPath[0].c_str(), &internal->audioSpec, &audioBuffer, &internal->audioLength) == NULL)
        {
            L_ERROR("Could not load audio file: {}", assetName);
        }

        // Commit audio buffer
        internal->audioBuffer.reset(audioBuffer);

        /** @todo: make sure map insert is atomic */
        audioDataCache.insert(std::make_pair(assetName, internal));
        L_DEBUG("Audio loaded from file: {}", assetName);
        return internal;
    }

    bool init()
    {
        L_TAG("AudioManager::init");

        // Verify SDL Audio was initialized
        if (!(SDL_WasInit(SDL_INIT_AUDIO) & SDL_INIT_AUDIO)) L_THROW_RUNTIME("SDL_INIT_AUDIO was not initialized");

        /** @todo: in SDL 2.24.0, use SDL_GetDefaultAudioInfo to query default audio device */

        // List audio devices
        int audioDeviceCount = SDL_GetNumAudioDevices(0);
        L_INFO("Found {} audio device(s)", audioDeviceCount);
        for (int i = 0; i < audioDeviceCount; ++i)
        {
            const char *deviceName = SDL_GetAudioDeviceName(i, 0);
            L_INFO("\t{}", deviceName);
        }

        SDL_AudioSpec want, have;
        SDL_memset(&want, 0, sizeof(want));
        want.freq     = defaultAudioFrequency;
        want.channels = defaultAudioChannel;
        want.samples  = defaultAudioSamples;
        want.format   = defaultAudioFormat;
        want.callback = audioCallback;
        // want.userdata = this;

        // Open default audio device
        SDL_AudioDeviceID devId = SDL_OpenAudioDevice(NULL, 0, &want, &have, audioDeviceAllowedChanges);
        if (devId)
        {
            audioDevice.deviceId   = devId;
            audioDevice.spec       = have;

            L_INFO("\t Frequency: {}Hz", have.freq);
            L_INFO("\t Channels:  {}", have.channels);
            L_INFO("\t Samples:   {}", have.samples);
            L_INFO("\t Format:    0x{:x}", have.format);
        }
        else
        {
            L_ERROR("Could not open default audio device");
        }

        // Start audio device playback
        SDL_PauseAudioDevice(audioDevice.deviceId, 0);

        return true;
    }

    /** @todo: add threading for update methods? */
    void preUpdate() {}

    void fixedUpdate(const time_ms &delta) {}

    void update(const time_ms &delta) {}

    void postUpdate() {}

    void refresh() {}
}; // namespace core::audio::manager