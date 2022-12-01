#pragma once

/**
 * @file core/ecs/components/audioComponent.hpp
 * @author Cedric Velandres (ccvelandres@gmail.com)
 *
 * @addtogroup Components
 * @{
 */

#include "../component.hpp"

/**
 * @brief The AudioComponent enables to play/hear audio
 *
 */
class AudioComponent : public Component
{
private:
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
     * @brief Attaches an audio clip to this component
     * @todo: add an audioClip class? to handle audio data and audio controls
     */
    void addAudioClip();

    friend Entity;
    friend EntityManager;
    friend ComponentManager;
};

/** @} endgroup Components */
