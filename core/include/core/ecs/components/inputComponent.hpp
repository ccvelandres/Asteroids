#pragma once

/**
 * @file core/ecs/components/inputComponent.hpp
 * @author Cedric Velandres (ccvelandres@gmail.com)
 * 
 * @addtogroup Components
 * @{
 */

#include "../component.hpp"
#include "../../input/input-types.hpp"

#include <functional>
#include <unordered_map>
#include <vector>

class InputManager; /** Forward declaration to InputManager */

/**
 * @brief The InputComponent interfaces with the Input system
 *
 */
class InputComponent : public Component
{
public:
    typedef std::function<void(const InputEventType, SDL_Event *)> Listener;
private:
    std::unordered_map<InputEventType, Listener> m_listeners;
protected:

    /** Protected Constructors (use entity to add components) */
    InputComponent();
public:

    ~InputComponent();
    InputComponent(InputComponent &o)             = delete;
    InputComponent &operator=(InputComponent &o)  = delete;
    InputComponent(InputComponent &&o)            = default;
    InputComponent &operator=(InputComponent &&o) = default;

    /** Component overrides */
    // void awake() override;
    void init() override;
    // void fixedUpdate(time_ms delta) override;
    // void update(time_ms delta) override;
    // void clean() override;

    void setListener(const InputEventType event, const Listener &callback);

    friend Entity;
    friend EntityManager;
    friend ComponentManager;
    friend InputManager;
};

/** @} endgroup Components */
