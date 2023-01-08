#include <core/assets/asset-inventory.hpp>
#include <core/audio/audioManager.hpp>
#include <core/utils/logging.hpp>
#include <core/utils/memory.hpp>
#include <core/ecs/components/audioComponent.hpp>
#include <audio/audio_p.hpp>

#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>

#include <glm/gtc/type_ptr.hpp>

namespace core::audio
{

    Audio::Audio(const AssetName &assetName, AudioComponent &audioComponent)
    {
        L_TAG("Audio::Audio");
        ALenum err;

        this->m_internal = std::make_unique<Internal>();
        L_ASSERT(this->m_internal, "Failed to allocate private internal data");

        this->m_internal->assetName      = assetName;
        this->m_internal->audioComponent = &audioComponent;

        this->m_internal->audioData = AudioManager::Instance().loadAudioFile(assetName);
        L_ASSERT(this->m_internal->audioData, "Failed to load audio data");

        // Generate source
        alGetError();
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

    Audio::~Audio()
    {
        L_TAG("Audio::~Audio");
        alDeleteSources((ALsizei)1, &this->m_internal->sourceId);
    }

    void Audio::play(const float &offset)
    {
        L_TAG("Audio::play");
        ALenum err;
        alGetError();

        alSourcef(this->m_internal->sourceId, AL_SEC_OFFSET, offset);
        if ((err = alGetError()) != AL_NO_ERROR)
        {
            L_ERROR("Could not set playback offset: {}", this->m_internal->assetName);
        }

        alSourcePlay(this->m_internal->sourceId);
        if ((err = alGetError()) != AL_NO_ERROR)
        {
            L_ERROR("Could not start playback: {}", this->m_internal->assetName);
        }
    }

    void Audio::stop()
    {
        L_TAG("Audio::stop");
        ALenum err;
        alGetError();

        alSourceStop(this->m_internal->sourceId);
        if ((err = alGetError()) != AL_NO_ERROR)
        {
            L_ERROR("Could not stop playback: {}", this->m_internal->assetName);
        }
    }

    void Audio::pause()
    {
        L_TAG("Audio::pause");
        ALenum err;
        alGetError();

        alSourcePause(this->m_internal->sourceId);
        if ((err = alGetError()) != AL_NO_ERROR)
        {
            L_ERROR("Could not pause playback: {}", this->m_internal->assetName);
        }
    }

    Audio &Audio::setPosition(const glm::vec3 &position) noexcept
    {
        L_TAG("Audio::setPosition");
        ALenum err;
        alGetError();
        alSourcefv(this->m_internal->sourceId, AL_POSITION, glm::value_ptr(position));
        if ((err = alGetError()) != AL_NO_ERROR)
        {
            L_ERROR("Could not set source position");
        }
        return *this;
    }

    Audio &Audio::setVelocity(const glm::vec3 &velocity) noexcept
    {
        L_TAG("Audio::setVelocity");
        ALenum err;
        alGetError();
        alSourcefv(this->m_internal->sourceId, AL_VELOCITY, glm::value_ptr(velocity));
        if ((err = alGetError()) != AL_NO_ERROR)
        {
            L_ERROR("Could not set source velocity");
        }
        return *this;
    }

    Audio &Audio::setDirection(const glm::vec3 &direction) noexcept
    {
        L_TAG("Audio::setDirection");
        ALenum err;
        alGetError();
        alSourcefv(this->m_internal->sourceId, AL_DIRECTION, glm::value_ptr(direction));
        if ((err = alGetError()) != AL_NO_ERROR)
        {
            L_ERROR("Could not set source direction");
        }
        return *this;
    }

    Audio &Audio::setVolume(const float &volume) noexcept
    {
        L_TAG("Audio::setVolume");
        ALenum err;
        alGetError();

        alSourcef(this->m_internal->sourceId, AL_GAIN, volume);
        if ((err = alGetError()) != AL_NO_ERROR)
        {
            L_ERROR("Could not set volume: {}", this->m_internal->assetName);
        }
        return *this;
    }

    Audio &Audio::setLoop(const bool &loop) noexcept
    {
        L_TAG("Audio::setLoop");
        ALenum err;
        ALint  value = loop ? AL_TRUE : AL_FALSE;
        alGetError();

        alSourcei(this->m_internal->sourceId, AL_LOOPING, value);
        if ((err = alGetError()) != AL_NO_ERROR)
        {
            L_ERROR("Could not set volume: {}", this->m_internal->assetName);
        }
        this->m_internal->isLooping = loop;
        return *this;
    }

    Audio &Audio::setRelative(const bool &relative) noexcept
    {
        L_TAG("Audio::setRelative");
        ALenum err;
        alGetError();

        alSourcef(this->m_internal->sourceId, AL_SOURCE_RELATIVE, relative);
        if ((err = alGetError()) != AL_NO_ERROR)
        {
            L_ERROR("Could not set position relative: {}", this->m_internal->assetName);
        }
        this->m_internal->isRelative = relative;
        return *this;
    }

    Audio &Audio::setOffset(const float &offset) noexcept
    {
        L_TAG("Audio::setOffset");
        ALenum err;
        alGetError();

        alSourcef(this->m_internal->sourceId, AL_SEC_OFFSET, offset);
        if ((err = alGetError()) != AL_NO_ERROR)
        {
            L_ERROR("Could not set playback offset: {}", this->m_internal->assetName);
        }
        return *this;
    }

    Audio &Audio::setAnchor(const bool &anchor) noexcept
    {
        L_TAG("Audio::setAnchor");
        this->m_internal->isAnchored = anchor;
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

    float Audio::getVolume() const noexcept
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
        return this->m_internal->isLooping;
    }

    std::size_t Audio::getLength() const noexcept
    {
        L_TAG("Audio::getLength");

        return this->m_internal->audioData->duration;
    }

    bool Audio::getRelative() const noexcept
    {
        L_TAG("Audio::getLength");
        return this->m_internal->isRelative;
    }

    float Audio::getOffset() const noexcept
    {
        L_TAG("Audio::getOffset");

        /** @todo: is it worth caching this values to internal struct */
        ALenum  err;
        ALfloat value;
        alGetError();
        alGetSourcef(this->m_internal->sourceId, AL_SEC_OFFSET, &value);
        if (err = alGetError() != AL_NO_ERROR)
        {
            L_ERROR("Error during query: {}", alGetString(err));
        }
        return value;
    }

    bool Audio::getAnchor() const noexcept
    {
        L_TAG("Audio::getAnchor");
        return this->m_internal->isAnchored;
    }

    const AssetName &Audio::getAssetName() const noexcept
    {
        L_TAG("Audio::getOffset");
        return this->m_internal->assetName;
    }

    AudioComponent &Audio::getComponent() const noexcept
    {
        L_TAG("Audio::getOffset");
        return *this->m_internal->audioComponent;
    }

    void Audio::update(time_ms delta)
    {
        L_TAG("Audio::update");
        TransformComponent *transform = this->m_internal->audioComponent->m_transform;

        // Anchored audio
        // If audio is anchored, position follows the attached transform component
        if (this->m_internal->isAnchored)
        {
            this->setPosition(transform->m_position);
        }

        // Directional sound
        // if direction vector is not == zero vector(0,0,0), sound is directional, else sound is omnidirectional
        if(this->m_internal->isDirectional)
        {
            this->setDirection(transform->getFront());
        }
    }

} // namespace core::audio