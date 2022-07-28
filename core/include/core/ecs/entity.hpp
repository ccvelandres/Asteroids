#pragma once

/**
 * @file core/ecs/entity.hpp
 * @author Cedric Velandres (ccvelandres@gmail.com)
 * 
 * @addtogroup ECS
 * @{
 */

#include "component.hpp"
#include "componentManager.hpp"
#include "../time.hpp"
#include "../utils/logging.hpp"

#include <array>
#include <bitset>
#include <memory>
#include <typeindex>

class EntityManager; /** Forward declartion for EntityManager */

using EntityID = std::type_index;

template <typename T = Entity, std::enable_if_t<std::is_base_of<Entity, T>::value, bool> = true>
using EntityPtr = std::unique_ptr<T>;

template <typename T, std::enable_if_t<std::is_base_of<Entity, T>::value, bool> = true>
inline EntityPtr<T> make_entity_ptr(const T *e)
{
    return EntityPtr<T>(e);
}

template <typename T, std::enable_if_t<std::is_base_of<Entity, T>::value, bool> = true>
constexpr EntityID getEntityID()
{
    return std::type_index(typeid(T));
}

template <typename T, std::enable_if_t<std::is_base_of<Entity, T>::value, bool> = true>
using EntityList = std::vector<T *>;

/**
 * @brief Base Class for all entities
 *
 */
class Entity
{
private:
    std::array<ComponentPtr<Component>, maxComponents> m_components;
    std::bitset<maxComponents>                         m_componentBitset;

    bool isActive = true;

    /**
     * @brief Registers component to the entity
     *
     * @param id componentID retrieved from getComponentID<T>()
     * @param component component to register
     */
    void addComponent(ComponentID id, const ComponentPtr<Component> &component);
protected:
    /** Protected Constructors (use EntityManager to add components) */
    Entity();
public:
    ~Entity();
    Entity(Entity &o)             = delete;
    Entity &operator=(Entity &o)  = delete;
    Entity(Entity &&o)            = default;
    Entity &operator=(Entity &&o) = default;

    /**
     * @brief Checks if entity has component of type @p T
     *
     * @tparam T type of component to check
     * @return true if entity has component of type T
     * @return false if entity has no component of type T
     */
    template <typename T, std::enable_if_t<std::is_base_of<Component, T>::value, bool> = true>
    bool hasComponent() noexcept
    {
        return m_componentBitset[getComponentID<T>()];
    }

    /**
     * @brief Removes component of type @p T attached to entity
     *
     * @tparam T type of component
     */
    template <typename T, std::enable_if_t<std::is_base_of<Component, T>::value, bool> = true>
    void removeComponent() noexcept
    {
        m_components[getComponentID<T>()].reset();
    }

    /**
     * @brief Creates and attaches component of type @p T to entity
     *
     * @tparam T type of component
     * @tparam TArgs parameter list for component constructor
     * @param args arguments forwarded to component constructor
     * @return T& reference to created component
     */
    template <typename T, typename... TArgs, std::enable_if_t<std::is_base_of<Component, T>::value, bool> = true>
    T &addComponent(TArgs &&...args)
    {
        L_TAG("Entity::addComponent");

        /** @todo: change return type to ComponentPtr<T> */
        L_ASSERT(hasComponent<T>() == false, "Entity already has component {}", typeid(T).name());

        T *c = new T(std::forward<TArgs>(args)...);
        L_ASSERT(c != NULL, "Failed to instantiate component of type {}", typeid(T).name());
        ComponentPtr<T> p(c);

        addComponent(getComponentID<T>(), p);
        c->m_entity = this;
        c->init();

        L_TRACE("Attached {} @ {} to instance of {} @ {}",
                L_TYPE_GETSTRING(T),
                static_cast<void *>(c),
                typeid(this).name(),
                static_cast<void *>(this));
        return *c;
    }

    /**
     * @brief Retrieves component of type @p T attached to entity
     *
     * @tparam T type of component
     * @return T& reference to component
     *
     * @throw std::logic_error if entity has no component of type @p T
     */
    template <typename T, std::enable_if_t<std::is_base_of<Component, T>::value, bool> = true>
    T &getComponent()
    {
        L_TAG("Entity::getComponent");

        /** @todo: change return type to ComponentPtr<T> */
        L_ASSERT(hasComponent<T>() == true, "Entity has no component", typeid(T).name());

        T *c = dynamic_cast<T *>(m_components[getComponentID<T>()].get());
        return *c;
    }

    /** Called once on allocation of entity */
    virtual void init() {}
    /** Called at the start of game update */
    virtual void preUpdate() {}
    /** Called for every frame update (uses unscaled time) */
    virtual void fixedUpdate(time_ms delta) {}
    /** Called for every frame update (uses scaled time) */
    virtual void update(time_ms delta) {}
    /** Called after updating most components */
    virtual void postUpdate() {}
    /** Called once when entity is reused */
    virtual void reset() {}
    /** Called once before sending object back to be reused */
    virtual void clean() {}

    friend EntityManager;
};

/** @} endgroup ECS */