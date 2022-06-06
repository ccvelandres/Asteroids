#pragma once

#include <functional>
#include <memory>
#include <map>
#include <vector>

#include <SDL2/SDL.h>

using EventType = std::size_t;

class EventHandler
{
public:
    virtual void onEvent(const EventType &ev) {}
};

class EventManager
{
private:
    std::map<EventType, std::vector<EventHandler>> m_eventListeners;

protected:
public:
    EventManager() {}

    void addListener(const EventType &evType,
                     const EventHandler &eventHandler)
    {
        m_eventListeners[evType].push_back(eventHandler);
    }

    void init()
    {
        SDL_SetEventFilter([](void *userdata, SDL_Event *event) -> int
                           {
            switch (event->type) {
                case SDL_KEYUP:
                case SDL_KEYDOWN:
                case SDL_TEXTEDITING:
                case SDL_TEXTINPUT:
                    return 0;
                default:
                    return 1;
            } },
                           NULL);
    }

    /** Poll SDL Events */
    void handleEvents()
    {
        SDL_PumpEvents();

        const uint8_t *state = SDL_GetKeyboardState(NULL);
        // if (state[SDL_SCANCODE_0])
        //     std::cout << "Keypress 0" << std::endl;
        // if (state[SDL_SCANCODE_1])
        //     std::cout << "Keypress 0" << std::endl;
    }
};