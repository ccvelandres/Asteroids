#include <core/assets/asset-inventory.hpp>
#include <core/audio/audioManager.hpp>
#include <core/utils/logging.hpp>
#include <core/utils/memory.hpp>
#include <core/ecs/components/audioComponent.hpp>
#include <audio/audioClip_p.hpp>

#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>

#include <vector>
#include <algorithm>

AudioClip::AudioClip(const AssetName &assetName, AudioComponent &component)
    : m_assetName(assetName),
      m_component(component)
{
    L_TAG("AudioClip::AudioClip");
    this->m_internal = AudioManager::getInstance().loadAudioFile(assetName);
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

AudioClip &AudioClip::setOffset(const std::size_t &offset) noexcept
{
    this->m_offset = offset;
    return *this;
}

uint8_t AudioClip::getVolume() const noexcept { return this->m_volume; }

uint8_t AudioClip::getFade() const noexcept { return this->m_fade; }

bool AudioClip::getLoop() const noexcept { return this->m_loop; }

std::size_t AudioClip::getLength() const noexcept { return this->m_length; }

std::size_t AudioClip::getOffset() const noexcept { return this->m_offset; }

AudioComponent &AudioClip::getComponent() const noexcept { return this->m_component; }