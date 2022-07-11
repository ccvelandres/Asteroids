#pragma once

/**
 * @addtogroup ECS
 * @{
 */

#include "common.hpp"
#include "../time.hpp"

#include <memory>

class Entity; /** Forward declaration for Base Entity Class */
class EntityManager; /** Forward declaration for EntityManager Class */
class ComponentManager; /** Forward declaration for ComponentManager Class */

/**
 * @brief Contains the Base Component Class for the ECS System
 * 
 */
class Component
{
private:
    Entity *m_entity; /** Owner entity of this component */
    bool    m_enabled;

protected:
    Component();
public:
    virtual ~Component(){};

    /** Called when the component is first created */
    virtual void init() {}
    /** Called for every frame update (uses unscaled time) */
    virtual void fixedUpdate(time_ms delta) {}
    /** Called for every frame update (uses scaled time) */
    virtual void update(time_ms delta) {}
    /** Called when the component is released */
    virtual void clean() {}

    /** Returns the entity attached to this component */
    Entity &entity() { return *m_entity; }

    /** Controls whether the component is updated/active  */
    inline bool &enabled() { return m_enabled; }

    friend Entity;
    friend EntityManager;
    friend ComponentManager;
};

template <typename T, std::enable_if_t<std::is_base_of<Component, T>::value, bool> = true>
using ComponentPtr = std::shared_ptr<T>; /** Alias for Component Pointer */

template <typename T, std::enable_if_t<std::is_base_of<Component, T>::value, bool> = true>
using ComponentList = std::vector<ComponentPtr<T>>; /** Alias for vector of ComponentPtr */

/** @} endgroup ECS */