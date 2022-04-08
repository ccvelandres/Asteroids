#pragma once

#include <manager.hpp>
#include <SDL2/SDL_scancode.h>

class InputManager : public Manager<InputManager> {
private:
    const uint8_t *keyState;
public:
    InputManager() {}

    bool isKeyPressed(const SDL_Scancode& scanCode);

    void init();
    void update();
};