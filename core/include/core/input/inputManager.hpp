#pragma once

/**
 * @file core/input/inputManager.hpp
 * @author Cedric Velandres (ccvelandres@gmail.com)
 */

/**
 * @defgroup Input
 * @brief Interface for Input system
 * @ingroup Core
 * @{
 */

#include "../time.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_scancode.h>

#include <vector>

class InputComponent; /** Forward declaration for InputComponent */

/**
 * @brief Manager for all Input related system
 * @todo: maybe replace SDL_Scancode with our own type
 *
 */
class InputManager
{
private:
    static InputManager *m_instance;
    const uint8_t       *keyState;
    std::vector<uint8_t> lastFrameKeyState;
    std::vector<SDL_Event> inputEvents;

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


    bool isPressed(const SDL_Scancode &scanCode);
    bool isTapped(const SDL_Scancode &scanCode);

    void keyDown(const SDL_Scancode &scanCode);
    void keyUp(const SDL_Scancode &scanCode);


    void init();
    void update();
    void postUpdate();
};

/** @} endgroup OpenGL */
