#pragma once

/**
 * @file core/audio/audio.hpp
 * @author Cedric Velandres (ccvelandres@gmail.com)
 *
 * @addtogroup Audio
 * @{
 */

#include "../assets/asset-inventory.hpp"
#include "../time.hpp"
#include <glm/glm.hpp>
#include <memory>

class AudioComponent;

namespace core::audio
{
    class AudioManager;

    class Audio
    {
    public:
    private:
        struct Internal;
        std::unique_ptr<Internal> m_internal;
    protected:
    public:
        static constexpr uint8_t MAX_VOLUME = 1.0f;
        static constexpr uint8_t MAX_FADE   = 0xFF;

        Audio() = delete;
        Audio(const AssetName &assetName, AudioComponent &audioComponent);

        ~Audio();
        Audio(Audio &o)             = delete;
        Audio &operator=(Audio &o)  = delete;
        Audio(Audio &&o)            = default;
        Audio &operator=(Audio &&o) = default;

        /**
         * @brief Play the current audio clip from offset
         * @param offset offset to start (expressed in seconds)
         */
        void play(const float &offset = 0);

        /** @brief Stop the current audio clip */
        void stop();
        /** @brief Pause the current audio clip */
        void pause();

        /**
         * @brief Set position of audio source
         *
         * @param position audio position
         * @return Audio& reference to this object
         */
        Audio &setPosition(const glm::vec3 &position) noexcept;

        /**
         * @brief Set velocity of audio source
         *
         * @param position audio velocity
         * @return Audio& reference to this object
         */
        Audio &setVelocity(const glm::vec3 &velocity) noexcept;

        /**
         * @brief Set direction of audio source
         *
         * @param position audio direction
         * @return Audio& reference to this object
         */
        Audio &setDirection(const glm::vec3 &direction) noexcept;

        /**
         * @brief Set the Volume of current audio clip
         *
         * @param volume volume (max = 1.0f)
         * @return AudioClip& reference to this object
         */
        Audio &setVolume(const float &volume) noexcept;

        /**
         * @brief Set to loop the audio clip
         *
         * @param loop loop
         * @return AudioClip& reference to this object
         */
        Audio &setLoop(const bool &loop) noexcept;

        /**
         * @brief Set the playback length
         *
         * @param length length (bytes)
         * @return AudioClip&
         */
        Audio &setLength(const std::size_t &length) noexcept;

        /**
         * @brief Set whether audio is relative
         * @note For global sounds, set audio to relative and set position to 0,0,0
         * @param relative audio
         * @return Audio&
         */
        Audio &setRelative(const bool &relative) noexcept;

        /**
         * @brief Set the playback offset
         *
         * @param offset offset to play (bytes)
         * @return AudioClip& reference to this object
         */
        Audio &setOffset(const float &offset) noexcept;

        /**
         * @brief Anchors the audio to the attached component
         *
         * @param anchor anchor state
         * @return Audio& reference to this object
         */
        Audio &setAnchor(const bool &anchor) noexcept;

        /** @brief Check if the audio clip is playing */
        bool isPlaying() const noexcept;
        /** @brief Get the current volume of audio clip */
        float getVolume() const noexcept;
        /** @brief Check if the audio clip is looped */
        bool getLoop() const noexcept;
        /** @brief Get the length of audio data in bytes */
        std::size_t getLength() const noexcept;
        /** @brief Get the current position relation */
        bool getRelative() const noexcept;
        /** @brief Get the current playback offset */
        float getOffset() const noexcept;
        /** @brief Get the current anchor state */
        bool getAnchor() const noexcept;

        /**
         * @brief Retrieve the asset name for this audio clip
         * @return const AssetName& reference to asset name
         */
        const AssetName &getAssetName() const noexcept;

        /**
         * @brief Retrieve the audioComponent this clip is attached to
         * @return AudioComponent& audioComponent
         */
        AudioComponent &getComponent() const noexcept;

        /**
         * @brief Perform audio updates.
         * If audio is relative, performs positional update for audio
         *
         * @param delta delta time in ms
         */
        void update(time_ms delta);

        friend AudioManager;
        friend AudioComponent;
    };

} // namespace core::audio

/** @} endgroup Audio */
