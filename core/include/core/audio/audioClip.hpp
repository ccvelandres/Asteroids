#pragma once

/**
 * @file core/audio/audioClip.hpp
 * @author Cedric Velandres (ccvelandres@gmail.com)
 *
 * @addtogroup Audio
 * @{
 */

#include "../assets/asset-inventory.hpp"

#include <memory>

class AudioManager;
class AudioComponent;

typedef std::size_t AudioID;

struct AudioData;

/**
 * @brief Describes audio type and attenuation via distance
 *
 */
enum class AudioType
{
    Global,
    Spatial
};

class AudioClip
{
private:
    const AssetName m_assetName;
    AudioType       m_type;
    AudioComponent &m_component;

    bool        m_playing;
    uint8_t     m_volume;
    uint8_t     m_fade;
    bool        m_loop;
    std::size_t m_length;
    std::size_t m_offset;

    struct Internal;
    std::unique_ptr<Internal> m_internal;
protected:
    // Use AudioComponent to create AudioClip
    AudioClip(const AssetName &assetName, AudioComponent &component);
public:

    ~AudioClip();
    AudioClip(AudioClip &o)             = delete;
    AudioClip &operator=(AudioClip &o)  = delete;
    AudioClip(AudioClip &&o)            = default;
    AudioClip &operator=(AudioClip &&o) = default;

    /**
     * @brief Play the current audio clip from offset
     * @param offset offset to start
     */
    void play(const std::size_t &offset = 0);

    /** @brief Stop the current audio clip */
    void stop();
    /** @brief Pause the current audio clip */
    void pause();

    /**
     * @brief Set the Volume of current audio clip
     *
     * @param volume volume (max = 255)
     * @return AudioClip& reference to this object
     */
    AudioClip &setVolume(const uint8_t &volume) noexcept;

    /**
     * @brief Set audio fade rate
     *
     * @param fade fade
     * @return AudioClip& reference to this object
     */
    AudioClip &setFade(const uint8_t &fade) noexcept;

    /**
     * @brief Set to loop the audio clip
     *
     * @param loop loop
     * @return AudioClip& reference to this object
     */
    AudioClip &setLoop(const bool &loop) noexcept;

    /**
     * @brief Set the playback length
     *
     * @param length length (bytes)
     * @return AudioClip&
     */
    AudioClip &setLength(const std::size_t &length) noexcept;

    /**
     * @brief Set the playback offset
     *
     * @param offset offset to play (bytes)
     * @return AudioClip& reference to this object
     */
    AudioClip &setOffset(const std::size_t &offset) noexcept;

    /** @brief Check if the audio clip is playing */
    bool isPlaying() const noexcept;
    /** @brief Get the current volume of audio clip */
    uint8_t getVolume() const noexcept;
    /** @brief Get the fade rate of audio clip */
    uint8_t getFade() const noexcept;
    /** @brief Check if the audio clip is looped */
    bool getLoop() const noexcept;
    /** @brief Get the length of audio data in bytes */
    std::size_t getLength() const noexcept;
    /** @brief Get the current playback offset */
    std::size_t getOffset() const noexcept;

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

/** @} endgroup Audio */
