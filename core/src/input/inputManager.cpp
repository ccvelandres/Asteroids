#include <core/input/inputManager.hpp>
#include <core/utils/logging.hpp>

#include <SDL2/SDL.h>

#include <unordered_map>

const std::unordered_map<SDL_EventType, bool> inputEventTypes = {
    {SDL_KEYDOWN,                  1},
    {SDL_KEYUP,                    1},
    {SDL_TEXTEDITING,              1},
    {SDL_TEXTINPUT,                1},
    {SDL_KEYMAPCHANGED,            1},
    {SDL_MOUSEMOTION,              1},
    {SDL_MOUSEBUTTONDOWN,          1},
    {SDL_MOUSEBUTTONUP,            1},
    {SDL_MOUSEWHEEL,               1},
    {SDL_JOYAXISMOTION,            1},
    {SDL_JOYBALLMOTION,            1},
    {SDL_JOYHATMOTION,             1},
    {SDL_JOYBUTTONDOWN,            1},
    {SDL_JOYBUTTONUP,              1},
    {SDL_JOYDEVICEADDED,           1},
    {SDL_JOYDEVICEREMOVED,         1},
    {SDL_CONTROLLERAXISMOTION,     1},
    {SDL_CONTROLLERBUTTONDOWN,     1},
    {SDL_CONTROLLERBUTTONUP,       1},
    {SDL_CONTROLLERDEVICEADDED,    1},
    {SDL_CONTROLLERDEVICEREMOVED,  1},
    {SDL_CONTROLLERDEVICEREMAPPED, 1},
    {SDL_FINGERDOWN,               1},
    {SDL_FINGERUP,                 1},
    {SDL_FINGERMOTION,             1},
    {SDL_DOLLARGESTURE,            1},
    {SDL_DOLLARRECORD,             1},
    {SDL_MULTIGESTURE,             1},
    {SDL_CLIPBOARDUPDATE,          1},
    {SDL_DROPFILE,                 1},
    {SDL_DROPTEXT,                 1},
    {SDL_DROPBEGIN,                1},
    {SDL_DROPCOMPLETE,             1}
};

InputManager *InputManager::m_instance = nullptr;

InputManager::InputManager() = default;
InputManager::~InputManager() { m_instance = nullptr; }

InputManager &InputManager::getInstance()
{
    /** @todo: use CAS for assignment? */
    if (m_instance) m_instance = new InputManager();
    return *m_instance;
}

void InputManager::init()
{
    int numKeys = 0;
    keyState    = SDL_GetKeyboardState(&numKeys);
    lastFrameKeyState.reserve(numKeys);
    /** @todo: do we setup event filters here for input events? */
}

void InputManager::update()
{
    L_TAG("InputManager::update");

    /** Clear current event queue (hopefully we dont need to reserve since clear doesn't free current memory) */
    inputEvents.clear();

    SDL_FilterEvents(
        [](void *userdata, SDL_Event *event) -> int {
            std::vector<SDL_Event> *inputEvents = static_cast<std::vector<SDL_Event>*>(userdata);

            /** @todo: should we make the map non-const and push non-filtered event types with 0 value ? */
            auto it = inputEventTypes.find(static_cast<SDL_EventType>(event->type));
            if (it != inputEventTypes.end() && it->second)
            {
                /** Push filtered input events to our the input event queue */
                inputEvents->push_back(*event);
                return 0;
            }
            return 1;
        },
        &inputEvents);

    /** Feed the current input event queue to input components */
    /** @todo: possible threaded optimization for input events */
    L_TRACE("Queue has {} input events", inputEvents.size());
}

void InputManager::postUpdate() { memcpy(lastFrameKeyState.data(), keyState, lastFrameKeyState.size()); }

bool InputManager::isPressed(const SDL_Scancode &scanCode) { return keyState[scanCode]; }

bool InputManager::isTapped(const SDL_Scancode &scanCode)
{
    return (!lastFrameKeyState[scanCode] && keyState[scanCode]);
}

void InputManager::keyDown(const SDL_Scancode &scanCode)
{ /** @todo Implement buffering of inputs */
}

void InputManager::keyUp(const SDL_Scancode &scanCode)
{ /** @todo Implement buffering of inputs */
}