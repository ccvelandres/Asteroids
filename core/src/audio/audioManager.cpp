#include <core/audio/audioManager.hpp>
#include <core/utils/logging.hpp>
#include <core/utils/memory.hpp>
#include <core/ecs/components/audioComponent.hpp>
#include <audio/audioClip_p.hpp>

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

/** @todo: control defaults depending on config */
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
static AudioDevice                           audioDevice;
static ComponentPtr<AudioComponent>          defaultAudioListener;
static std::vector<std::weak_ptr<AudioClip>> audioClips;

static std::unordered_map<AssetName, std::weak_ptr<AudioClip::Internal>> audioClipCache;

AudioManager::AudioManager()  = default;
AudioManager::~AudioManager() = default;

AudioManager &AudioManager::getInstance()
{
    /** Meyers Singleton (another option is std::call_once) */
    static AudioManager instance = AudioManager();
    return instance;
}

static void audioCallback(void *userdata, Uint8 *stream, int len) {}

std::shared_ptr<AudioClip::Internal> AudioManager::loadAudioFile(const AssetName &assetName)
{
    L_TAG("AudioManager::loadAudioFile");
    std::shared_ptr<AudioClip::Internal> internal;

    // Check if asset was loaded already
    auto idCache = audioClipCache.find(assetName);
    if(idCache != audioClipCache.end())
    {
        internal = idCache->second.lock();
        if(internal) {
            L_DEBUG("Audio loaded from cache: {}", assetName);
            return internal;
        }
    }

    // Load asset to memory
    internal = std::make_shared<AudioClip::Internal>(AudioClip::Internal());
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
    audioClipCache.insert(std::make_pair(assetName, internal));
    L_DEBUG("Audio loaded from file: {}", assetName);
    return internal;
}

void AudioManager::init()
{
    L_TAG("AudioManager::init");

    // Verify SDL Audio was initialized
    if (!(SDL_WasInit(SDL_INIT_AUDIO) & SDL_INIT_AUDIO)) L_THROW_RUNTIME("SDL_INIT_AUDIO was not initialized");

    SDL_AudioSpec want, have;
    SDL_memset(&want, 0, sizeof(want));
    want.freq     = defaultAudioFrequency;
    want.channels = defaultAudioChannel;
    want.samples  = defaultAudioSamples;
    want.format   = defaultAudioFormat;
    want.callback = audioCallback;
    want.userdata = &audioDevice;

    // Open default audio device
    SDL_AudioDeviceID devId = SDL_OpenAudioDevice(NULL, 0, &want, &have, audioDeviceAllowedChanges);
    if (devId)
    {
        L_INFO("Using default audio device");
        L_INFO("\t Frequency: {}Hz", have.freq);
        L_INFO("\t Channels:  {}", have.channels);
        L_INFO("\t Samples:   {}", have.samples);
        L_INFO("\t Format:    0x{:x}", have.format);

        audioDevice.deviceId = devId;
        audioDevice.spec     = have;
    }
    else
    {
        L_ERROR("Could not open default audio device");
    }
}

void AudioManager::preUpdate() { L_TAG("AudioManager::preUpdate"); }

void AudioManager::fixedUpdate(const time_ms &delta) { L_TAG("AudioManager::fixedUpdate"); }

void AudioManager::update(const time_ms &delta) { L_TAG("AudioManager::update"); }

void AudioManager::postUpdate() { L_TAG("AudioManager::postUpdate"); }

void AudioManager::refresh() { L_TAG("AudioManager::refresh"); }