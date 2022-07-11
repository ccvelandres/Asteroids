#pragma once

/**
 * @addtogroup ECS
 * @{
 */

#include "common.hpp"
#include "componentManager.hpp"
#include "../game.hpp"
#include "../time.hpp"
#include "../utils/logging.hpp"

#include <array>
#include <bitset>
#include <memory>

class EntityManager; /** Forward declaration for the EntityManager */

/**
 * @brief Base Class for all entities
 *
 */
class Entity
{
private:
    std::array<std::shared_ptr<Component>, maxComponents> m_components;
    std::bitset<maxComponents>                            m_componentBitset;

    bool isActive = true;

    /** Disable copy-constructors */
    Entity(Entity &o)            = delete;
    Entity &operator=(Entity &o) = delete;
protected:
    Entity() = default; /** Use entity manager to create entities */
public:
    /** Allow move constructors */
    Entity(Entity &&o)            = default;
    Entity &operator=(Entity &&o) = default;
    ~Entity()                     = default;

    /**
     * @brief Checks if entity has component of type @p T
     *
     * @tparam T type of component to check
     * @return true if entity has component of type T
     * @return false if entity has no component of type T
     */
    template <typename T>
    bool hasComponent() noexcept
    {
        return m_componentBitset[getComponentID<T>()];
    }

    /**
     * @brief Removes component of type @p T attached to entity
     *
     * @tparam T type of component
     */
    template <typename T>
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
    template <typename T, typename... TArgs>
    T &addComponent(TArgs &&...args)
    {
        /** @todo: change return type to ComponentPtr<T> */

        T *c = nullptr;
        if (!hasComponent<T>())
        {
            c = new T(std::forward<TArgs>(args)...);
            std::shared_ptr<Component> p(c);

            m_components[getComponentID<T>()]      = p;
            m_componentBitset[getComponentID<T>()] = true;
            /** @todo: refactor so we don't depend on Game class? */
            Game::componentManager()->registerComponent<T>(p);
        }
        else
        {
            c = dynamic_cast<T *>(m_components[getComponentID<T>()].get());
        }

        c->m_entity = this;
        c->init();

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
    template <typename T>
    T &getComponent()
    {
        /** @todo: change return type to ComponentPtr<T> */
        assert(hasComponent<T>() == true);

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

template <typename T, std::enable_if_t<std::is_base_of<Entity, T>::value, bool> = true>
using EntityList = std::vector<T *>;

/** @} endgroup ECS */