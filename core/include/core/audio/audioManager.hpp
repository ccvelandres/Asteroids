#pragma once

/**
 * @file core/audio/audioManager.hpp
 * @author Cedric Velandres (ccvelandres@gmail.com)
 * 
 * @addtogroup Audio
 * @{
 */

#include "../time.hpp"
#include "../ecs/component.hpp"
#include "../ecs/components/audioComponent.hpp"

constexpr uint8_t maxVolume = 0xFF;

/**
 * @brief Manager for all audio related system
 *
 */
class AudioManager
{
private:

    /** AudioManager is a singleton */
    AudioManager();
protected:
public:
    ~AudioManager();
    AudioManager(AudioManager &o)             = delete;
    AudioManager(AudioManager &&o)            = delete;
    AudioManager &operator=(AudioManager &o)  = delete;
    AudioManager &operator=(AudioManager &&o) = delete;

    /**
     * @brief Get the Instance object
     *
     * @return AudioManager& reference to AudioManager
     */
    static AudioManager &getInstance();

    void setAudioListener();

    std::shared_ptr<AudioClip> createAudioClip(const AssetName &assetName, AudioComponent &component);

    void init();
    void preUpdate();
    void fixedUpdate(const time_ms &delta);
    void update(const time_ms &delta);
    void postUpdate();
    void refresh();
};

/** @} endgroup Audio */
