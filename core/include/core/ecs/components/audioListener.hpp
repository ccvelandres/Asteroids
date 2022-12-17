#pragma once

/**
 * @file core/ecs/components/audioListener.hpp
 * @author Cedric Velandres (ccvelandres@gmail.com)
 *
 * @addtogroup Components
 * @{
 */

#include "../../assets/asset-inventory.hpp"
#include "../component.hpp"
#include "transformComponent.hpp"

// Forward declare audio manager
namespace core::audio
{
    class AudioManager;
}

/**
 * @brief The AudioListener enables to hear audio
 *
 */
class AudioListener : public Component
{
private:
    TransformComponent                                  *m_transform; /** the audio's transform component */
protected:
    /** Protected Constructors (use entity to add components) */
    AudioListener();
public:

    ~AudioListener();
    AudioListener(AudioListener &o)             = delete;
    AudioListener &operator=(AudioListener &o)  = delete;
    AudioListener(AudioListener &&o)            = default;
    AudioListener &operator=(AudioListener &&o) = default;

    /** Component overrides */
    // void awake() override;
    void init() override;
    // void fixedUpdate(time_ms delta) override;
    // void update(time_ms delta) override;
    // void clean() override;

    /**
     * @brief Sets the audio listener in the scene to this current component
     * @todo: add option for 3D, 2D, global sounds
     */
    void listen();

    friend Entity;
    friend EntityManager;
    friend ComponentManager;
    friend class core::audio::AudioManager;
};

/** @} endgroup Components */
