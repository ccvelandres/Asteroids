#pragma once

/**
 * @file core/ecs/components/audioComponent.hpp
 * @author Cedric Velandres (ccvelandres@gmail.com)
 *
 * @addtogroup Components
 * @{
 */

#include "../../audio/audioClip.hpp"
#include "../../assets/asset-inventory.hpp"
#include "../component.hpp"

class AudioManager;

/**
 * @brief The AudioComponent enables to play/hear audio
 *
 */
class AudioComponent : public Component
{
private:
    std::vector<std::shared_ptr<core::audio::AudioClip>> m_audioClips;
protected:
    /** Protected Constructors (use entity to add components) */
    AudioComponent();
public:

    ~AudioComponent();
    AudioComponent(AudioComponent &o)             = delete;
    AudioComponent &operator=(AudioComponent &o)  = delete;
    AudioComponent(AudioComponent &&o)            = default;
    AudioComponent &operator=(AudioComponent &&o) = default;

    /** Component overrides */
    // void awake() override;
    // void init() override;
    // void fixedUpdate(time_ms delta) override;
    // void update(time_ms delta) override;
    // void clean() override;

    /**
     * @brief Attaches a listener to the attached object. Only one listener can exist
     * in a scene
     * @todo: add option for 3D, 2D, global sounds
     */
    void addListener();

    /**
     * @brief Attaches an audio clip to this component. The AudioClip lifespan is the same as the
     * audioComponent that owns it.
     * @todo: add an audioClip class? to handle audio data and audio controls
     *
     * @return AudioClip& reference to created audio clip
     */
    core::audio::AudioClip &addAudioClip(const AssetName &audioName, const core::audio::AudioType &audioType);

    /**
     * @brief Retrieves a pointer array to all audio clips attached to this component
     *
     * @return std::vector<AudioClip*>
     */
    std::vector<core::audio::AudioClip *> getAudioClips();

    core::audio::AudioClip &operator[](std::size_t id);

    friend Entity;
    friend EntityManager;
    friend ComponentManager;
};

/** @} endgroup Components */
