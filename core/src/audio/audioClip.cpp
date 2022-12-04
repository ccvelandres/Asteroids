#include <core/assets/asset-inventory.hpp>
#include <core/audio/audioManager.hpp>
#include <core/utils/logging.hpp>
#include <core/utils/memory.hpp>
#include <core/ecs/components/audioComponent.hpp>
#include <audio/audioManager_p.hpp>

#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>

#include <vector>
#include <algorithm>

AudioClip::AudioClip(const AssetName &assetName, AudioComponent &component)
    : m_assetName(assetName),
      m_component(component)
{
    L_TAG("AudioClip::AudioClip");

    std::unique_ptr<Internal> internal  = std::make_unique<Internal>(AudioClip::Internal());
    auto      audioPath = AssetInventory::getInstance().resolvePath(AssetType::Audio, assetName);
    L_ASSERT(audioPath.size() == 1, "Found multiple paths for file: {}", assetName);

    uint8_t *audioBuffer;
    if (SDL_LoadWAV(audioPath[0].c_str(), &internal->audioSpec, &audioBuffer, &internal->audioLength) == NULL)
    {
        L_ERROR("Could not load audio file: {}", assetName);
    }

    // Commit audio buffer
    internal->audioBuffer.reset(audioBuffer);
    internal->playbackOffset = 0;
    L_DEBUG("Audio file loaded: {}", assetName);

    this->m_internal.swap(internal);
}

AudioClip &AudioClip::setVolume(const uint8_t &volume) noexcept
{
    this->m_volume = volume;
    return *this;
}

AudioClip &AudioClip::setFade(const uint8_t &fade) noexcept
{
    this->m_fade = fade;
    return *this;
}

AudioClip &AudioClip::setLoop(const bool &loop) noexcept
{
    this->m_loop = loop;
    return *this;
}

AudioClip &AudioClip::setLength(const std::size_t &length) noexcept
{
    this->m_length = length;
    return *this;
}

uint8_t AudioClip::getVolume() const noexcept { return this->m_volume; }

uint8_t AudioClip::getFade() const noexcept { return this->m_fade; }

bool AudioClip::getLoop() const noexcept { return this->m_loop; }

std::size_t AudioClip::getLength() const noexcept { return this->m_length; }

AudioComponent &AudioClip::getComponent() const noexcept { return this->m_component; }