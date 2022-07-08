#pragma once
#include <vector>
#include "../manager.hpp"
#include <SDL2/SDL_scancode.h>

class InputManager
{
private:
    const uint8_t       *keyState;
    std::vector<uint8_t> lastFrameKeyState;
public:
    InputManager() {}

    bool isPressed(const SDL_Scancode &scanCode);
    bool isTapped(const SDL_Scancode &scanCode);

    void keyDown(const SDL_Scancode &scanCode);
    void keyUp(const SDL_Scancode &scanCode);

    void init();
    void update();
    void postUpdate();
};