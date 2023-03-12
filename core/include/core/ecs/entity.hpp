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

#include <unordered_map>
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
    /** @todo: wrap componentptr with atomic (C++20) */
    std::unordered_map<ComponentID, ComponentPtr<Component>> m_components;

    bool isActive = true;

    /**
     * @brief Registers component to the entity
     *
     * @param id componentID retrieved from getComponentID<T>()
     * @param component component to register
     * @return true if component was successfully registered, otherwise false
     */
    void addComponent(const ComponentID &id, ComponentPtr<Component> &component);

    /**
     * @brief Checks if entity has component of id @p id
     *
     * @param id component id
     * @return true if entity has component with @p id
     */
    bool hasComponent(const ComponentID &id) const noexcept;

    /**
     * @brief Removes attached component with @p id from the entity
     *
     * @param id component id
     */
    void removeComponent(const ComponentID &id);

    /**
     * @brief Retrieves component of @p id attached to this entity
     *
     * @param  id component id
     */
    ComponentPtr<Component> getComponent(const ComponentID &id) const;
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
        return this->hasComponent(getComponentID<T>());
    }

    /**
     * @brief Removes component of type @p T attached to entity
     *
     * @tparam T type of component
     */
    template <typename T, std::enable_if_t<std::is_base_of<Component, T>::value, bool> = true>
    void removeComponent() noexcept
    {
        this->removeComponent(getComponentID<T>());
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
        auto id = getComponentID<T>();

        /** @todo: change return type to ComponentPtr<T> */
        L_ASSERT(hasComponent<T>() == false, "Entity already has component {}", typeid(T).name());

        ComponentPtr<Component> p = std::make_shared<T>(T(std::forward<TArgs>(args)...));
        L_ASSERT(p, "Failed to instantiate component of type {}", typeid(T).name());

        addComponent(id, p);
        p->m_entity = this;
        p->init();

        L_TRACE("Attached {}:{} @ {} to instance of entity @ {}",
                L_TYPE_GETSTRING(T),
                id.hash_code(),
                static_cast<void *>(p.get()),
                static_cast<void *>(this));
        return static_cast<T &>(*p);
    }

    /**
     * @brief Retrieves component of type @p T attached to entity
     *
     * @tparam T type of component
     * @return T& reference to component
     *
     * @throw std::logic_error if entity has no component of type @p T
     * @{
     */
    template <typename T, std::enable_if_t<std::is_base_of<Component, T>::value, bool> = true>
    ComponentPtr<T> getComponentPtr()
    {
        auto ptr = this->getComponent(getComponentID<T>());
        return std::static_pointer_cast<T>(ptr);
    }
    template <typename T, std::enable_if_t<std::is_base_of<Component, T>::value, bool> = true>
    T &getComponent()
    {
        return *(getComponentPtr<T>().get());
    }
    /** @} */

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