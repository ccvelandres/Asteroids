#include <core/assets/asset-inventory.hpp>
#include <core/audio/audioManager.hpp>
#include <core/utils/logging.hpp>
#include <core/utils/memory.hpp>
#include <core/ecs/components/audioComponent.hpp>
#include <audio/audio_p.hpp>

#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>

#include <vector>
#include <algorithm>

namespace core::audio
{

    Audio::Audio(const AssetName &assetName, AudioComponent &audioComponent)
    {
        L_TAG("Audio::Audio");
        ALenum err;

        this->m_internal = std::make_unique<Internal>(Internal{.assetName = assetName, .audioComponent = audioComponent});
        L_ASSERT(this->m_internal, "Failed to allocate private internal data");

        this->m_internal->audioData = AudioManager::Instance().loadAudioFile(assetName);
        L_ASSERT(this->m_internal->audioData, "Failed to load audio data");

        this->m_internal->isPlaying = false;
        this->m_internal->transform = audioComponent.m_transform;

        alGetError();

        // Generate source
        alGenSources((ALsizei)1, &this->m_internal->sourceId);
        if ((err = alGetError()) != AL_NO_ERROR)
        {
            L_THROW_RUNTIME("Could not generate al source");
        }

        // Attach buffer to source
        alSourcei(this->m_internal->sourceId, AL_BUFFER, this->m_internal->audioData->bufferId);
        if ((err = alGetError()) != AL_NO_ERROR)
        {
            L_THROW_RUNTIME("Could not attach audio buffer to source");
        }
    }

    Audio::~Audio() = default;

    void Audio::play(const std::size_t &offset)
    {
        // this->m_offset = offset;
        core::audio::AudioManager::Instance().playAudio(*this);
    }

    void Audio::stop() {}

    void Audio::pause() {}

    Audio &Audio::setVolume(const uint8_t &volume) noexcept
    {
        // this->m_volume = volume;
        return *this;
    }

    Audio &Audio::setLoop(const bool &loop) noexcept
    {
        // this->m_internal->sourceData.loop = loop;
        return *this;
    }

    Audio &Audio::setLength(const std::size_t &length) noexcept
    {
        // this->m_length = length;
        return *this;
    }

    Audio &Audio::setOffset(const std::size_t &offset) noexcept
    {
        // this->m_offset = offset;
        return *this;
    }

    bool Audio::isPlaying() const noexcept
    {
        L_TAG("Audio::isPlaying");

        /** @todo: is it worth caching this values to internal struct */
        ALenum err;
        ALint  value;
        alGetError();
        alGetSourcei(this->m_internal->sourceId, AL_SOURCE_STATE, &value);
        if (err = alGetError() != AL_NO_ERROR)
        {
            L_ERROR("Error during source query: {}", alGetString(err));
        }
        return value == AL_PLAYING ? true : false;
    }

    uint8_t Audio::getVolume() const noexcept
    {
        L_TAG("Audio::getVolume");

        /** @todo: is it worth caching this values to internal struct */
        ALenum  err;
        ALfloat value;
        alGetError();
        alGetSourcef(this->m_internal->sourceId, AL_GAIN, &value);
        if (err = alGetError() != AL_NO_ERROR)
        {
            L_ERROR("Error during query: {}", alGetString(err));
        }
        return value;
    }

    bool Audio::getLoop() const noexcept
    {
        L_TAG("Audio::getLoop");
        /** @todo: is it worth caching this values to internal struct */
        ALenum err;
        ALint  value;
        alGetError();
        alGetSourcei(this->m_internal->sourceId, AL_LOOPING, &value);
        if (err = alGetError() != AL_NO_ERROR)
        {
            L_ERROR("Error during query: {}", alGetString(err));
        }
        return value == AL_TRUE ? true : false;
    }

    std::size_t Audio::getLength() const noexcept
    {
        L_TAG("Audio::getLength");

        return this->m_internal->audioData->duration;
    }

    std::size_t Audio::getOffset() const noexcept
    {
        L_TAG("Audio::getOffset");

        /** @todo: is it worth caching this values to internal struct */
        ALenum  err;
        ALfloat value;
        alGetError();
        alGetSourcef(this->m_internal->sourceId, AL_GAIN, &value);
        if (err = alGetError() != AL_NO_ERROR)
        {
            L_ERROR("Error during query: {}", alGetString(err));
        }
        return value;
    }

    const AssetName &Audio::getAssetName() const noexcept { return this->m_internal->assetName; }

    AudioComponent &Audio::getComponent() const noexcept { return this->m_internal->audioComponent; }

} // namespace core::audio