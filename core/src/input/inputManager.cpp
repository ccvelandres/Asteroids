#include <input/inputManager.hpp>
#include <cassert>

#include <SDL2/SDL.h>

void InputManager::init() {
    int numKeys = 0;
    keyState = SDL_GetKeyboardState(&numKeys);
    lastFrameKeyState.reserve(numKeys);
}

void InputManager::update() {
    /** @todo: if input buffered, update internal state here */
}

void InputManager::postUpdate() {
    memcpy(lastFrameKeyState.data(), keyState, lastFrameKeyState.size());
}

bool InputManager::isPressed(const SDL_Scancode& scanCode) {
    return keyState[scanCode];
}

bool InputManager::isTapped(const SDL_Scancode& scanCode) {
    return (!lastFrameKeyState[scanCode] && keyState[scanCode]);
}

void InputManager::keyDown(const SDL_Scancode &scanCode) {
    /** @todo Implement buffering of inputs */
    assert(0);
}

void InputManager::keyUp(const SDL_Scancode &scanCode) {
    /** @todo Implement buffering of inputs */
    assert(0);
}