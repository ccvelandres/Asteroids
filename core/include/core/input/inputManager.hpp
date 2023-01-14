#pragma once

/**
 * @file core/input/inputManager.hpp
 * @author Cedric Velandres (ccvelandres@gmail.com)
 * 
 * @addtogroup Input
 * @{
 */

#include "../time.hpp"
#include "../ecs/component.hpp"
#include "../ecs/components/inputComponent.hpp"

#include <SDL.h>
#include <SDL_scancode.h>

#include <vector>
#include <memory>

/**
 * @brief Manager for all Input related system
 * @todo: maybe replace SDL_Scancode with our own type and reduce dependence on SDL2 (at least on upper levels)
 *
 */
class InputManager
{
private:
    const uint8_t         *m_keyState;
    std::vector<uint8_t>   m_lastFrameKeyState;
    std::vector<SDL_Event> m_inputEvents;

    /** InputManager is a singleton */
    InputManager();
protected:
public:
    ~InputManager();
    InputManager(InputManager &o)             = delete;
    InputManager(InputManager &&o)            = delete;
    InputManager &operator=(InputManager &o)  = delete;
    InputManager &operator=(InputManager &&o) = delete;

    /**
     * @brief Get the Instance object
     *
     * @return InputManager& reference to InputManager
     */
    static InputManager &getInstance();

    void init();
    void preUpdate();
    void fixedUpdate(const time_ms &delta);
    void update(const time_ms &delta);
    void postUpdate();
    void refresh();

    bool isPressed(const SDL_Scancode scanCode);
    bool isTapped(const SDL_Scancode scanCode);
    bool getKeyUp(const SDL_Scancode scanCode);
    bool getKeyDown(const SDL_Scancode scanCode);
};

/** @} endgroup Input */
