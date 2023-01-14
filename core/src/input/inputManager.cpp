#include <core/input/inputManager.hpp>
#include <core/utils/logging.hpp>
#include <core/ecs/componentManager.hpp>

#include <SDL.h>

#include <unordered_map>

const std::unordered_map<SDL_EventType, int> inputEventTypes = {
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

InputManager::InputManager()  = default;
InputManager::~InputManager() = default;

InputManager &InputManager::getInstance()
{
    /** Meyers Singleton (another option is std::call_once) */
    static InputManager instance = InputManager();
    return instance;
}

void InputManager::init()
{
    L_TAG("InputManager::init");

    int numKeys = 0;
    m_keyState  = SDL_GetKeyboardState(&numKeys);
    m_lastFrameKeyState.reserve(numKeys);
    /** @todo: do we setup event filters here for input events? */
}

void InputManager::preUpdate()
{
    L_TAG("InputManager::preUpdate");

    /** @todo: add support for controllers
     * initialize or cleanup when # of joysticks mismatched
     */

    /** Clear current event queue (hopefully we dont need to reserve since clear doesn't free current memory) */
    m_inputEvents.clear();

    SDL_FilterEvents(
        [](void *userdata, SDL_Event *event) -> int {
            std::vector<SDL_Event> *inputEvents = static_cast<std::vector<SDL_Event> *>(userdata);

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
        &m_inputEvents);
    if (m_inputEvents.size()) L_TRACE_RATE(32, "Queue has {} input events", m_inputEvents.size());
}

void InputManager::fixedUpdate(const time_ms &delta) {}

void InputManager::update(const time_ms &delta)
{
    L_TAG("InputManager::update");
    /** Feed the current input event queue to input components */
    /** @todo: possible threaded optimization for input events */
    for (auto &ev : m_inputEvents)
    {
        ComponentManager::getInstance().foreach<InputComponent>([&ev](InputComponent &c) -> void {
            auto f = c.m_listeners.find(static_cast<InputEventType>(ev.type));
            if (f != c.m_listeners.end()) f->second(static_cast<InputEventType>(ev.type), &ev);
        });
    }
}

void InputManager::postUpdate()
{
    L_TAG("InputManager::postUpdate");
    /** Cache current keyboard state so we can compare for last state change */
    memcpy(m_lastFrameKeyState.data(), m_keyState, m_lastFrameKeyState.size());
}

bool InputManager::isPressed(const SDL_Scancode scanCode) { return m_keyState[scanCode]; }

bool InputManager::isTapped(const SDL_Scancode scanCode)
{
    return (!m_lastFrameKeyState[scanCode] && m_keyState[scanCode]);
}