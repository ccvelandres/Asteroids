#pragma once

/**
 * @file core/audio/audio.hpp
 * @author Cedric Velandres (ccvelandres@gmail.com)
 *
 * @addtogroup Audio
 * @{
 */

#include "../assets/asset-inventory.hpp"

#include <memory>

class AudioComponent;

namespace core::audio
{
    class AudioManager;

    /**
     * @brief Describes audio type and attenuation via distance
     *
     */
    enum class AudioType
    {
        Global,
        Spatial
    };

    class Audio
    {
    public:
    private:
        struct Internal;
        std::unique_ptr<Internal> m_internal;
    protected:
        // Use AudioComponent to create AudioClip
        Audio(const AssetName &assetName, AudioComponent &audioComponent);
    public:

        static constexpr uint8_t MAX_VOLUME = 1.0f;
        static constexpr uint8_t MAX_FADE   = 0xFF;

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
         * @brief Set the playback offset
         *
         * @param offset offset to play (bytes)
         * @return AudioClip& reference to this object
         */
        Audio &setOffset(const float &offset) noexcept;

        /** @brief Check if the audio clip is playing */
        bool isPlaying() const noexcept;
        /** @brief Get the current volume of audio clip */
        float getVolume() const noexcept;
        /** @brief Check if the audio clip is looped */
        bool getLoop() const noexcept;
        /** @brief Get the length of audio data in bytes */
        std::size_t getLength() const noexcept;
        /** @brief Get the current playback offset */
        float getOffset() const noexcept;

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
         * @brief Free the resources for this audio clip. After this call, the audio clip object is no longer valid.
         * @note The audio clip may be cached and referenced by multiple audio clip objects. In this case, the shared
         * resource is only freed when the last object is freed
         */
        void free();

        friend AudioManager;
        friend AudioComponent;
    };

} // namespace core::audio

/** @} endgroup Audio */
