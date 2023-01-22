#pragma once

/**
 * @file core/ecs/component.hpp
 * @author Cedric Velandres (ccvelandres@gmail.com)
 * 
 * @addtogroup ECS
 * @{
 */

#include "../time.hpp"

#include <memory>
#include <vector>

class Component; /** Forward declaration for Component Class */
class Entity; /** Forward declaration for Base Entity Class */
class EntityManager; /** Forward declaration for EntityManager Class */
class ComponentManager; /** Forward declaration for ComponentManager Class */

using ComponentID                   = std::size_t;
constexpr std::size_t maxComponents = 32;

/**
 * @todo Replace component ID with no RTTI
 *
 * template<typename T>
 * struct type { static void id() { } };
 *
 * template<typename T>
 * std::size_t type_id() { return reinterpret_cast<std::size_t>(&type<T>::id); }
 */
inline ComponentID getComponentID()
{
    static ComponentID lastID = 0;
    return lastID++;
}

template <typename T>
inline ComponentID getComponentID() noexcept
{
    static ComponentID cID = getComponentID();
    return cID;
}

template <typename T = Component, std::enable_if_t<std::is_base_of<Component, T>::value, bool> = true>
using ComponentPtr = std::shared_ptr<T>; /** Alias for Component Pointer */

template <typename T = Component, std::enable_if_t<std::is_base_of<Component, T>::value, bool> = true>
using ComponentList = std::vector<ComponentPtr<T>>; /** Alias for vector of ComponentPtr */


/**
 * @brief Contains the Base Component Class for the ECS System
 * 
 */
class Component
{
private:
protected:
    bool    m_enabled;
    Entity *m_entity; /** Owner entity of this component */
    Component();
public:
    virtual ~Component(){};

    /** Called when the component is first created */
    virtual void init() {}
    /** Called for every frame update (uses unscaled time) */
    virtual void fixedUpdate(time_ms delta) {}
    /** Called for every frame update (uses scaled time) */
    virtual void update(time_ms delta) {}
    /** Called for every frame update (for preparing for rendering */
    virtual void postUpdate() {}
    /** Called when the component is released */
    virtual void clean() {}

    /** Returns the entity attached to this component */
    Entity &entity() { return *m_entity; }

    /** Controls whether the component is updated/active  */
    inline const bool enabled() { return m_enabled; }

    friend Entity;
    friend EntityManager;
    friend ComponentManager;
};

/** @} endgroup ECS */