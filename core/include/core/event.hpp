#pragma once

#include <functional>
#include <memory>
#include <map>
#include <vector>

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

    void addListener(const EventType &evType, const EventHandler &eventHandler)
    {
        m_eventListeners[evType].push_back(eventHandler);
    }

    void init() {}
};