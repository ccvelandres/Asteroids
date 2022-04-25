#pragma once

#include <memory>
#include <map>
#include <vector>
#include <array>
#include <algorithm>
#include <bitset>
#include <functional>
#include <iostream>
#include <typeindex>

#include <game.hpp>
#include <time.hpp>

/**
 * @brief ECS Component system
 *
 * @note Good library for ecs system -> https://github.com/skypjack/entt
 */

class Component;
class Entity;
class EntityManager;

/**
 * @todo Replace component ID with no RTTI
 *
 * template<typename T>
 * struct type { static void id() { } };
 *
 * template<typename T>
 * std::size_t type_id() { return reinterpret_cast<std::size_t>(&type<T>::id); }
 */

using ComponentID = std::size_t;
constexpr std::size_t maxComponents = 32;

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

class Component
{
private:
    Entity *m_entity;
    bool m_enabled;

protected:
public:
    Component() : m_enabled(true) {}
    virtual ~Component(){};

    /** Called when the component is first created */
    virtual void init() {}
    /** Called for every frame update (uses unscaled time) */
    virtual void fixedUpdate(time_ds delta) {}
    /** Called for every frame update (uses scaled time) */
    virtual void update(time_ds delta) {}
    /** Called when the component is released */
    virtual void clean() {}

    /** Returns the entity attached to this component */
    Entity &entity() { return *m_entity; }

    /** Returns status of component */
    inline bool enabled() { return m_enabled; }
    /** Set enable status of component */
    inline void enabled(const bool &enabled) { m_enabled = enabled; }

    friend Entity;
    friend EntityManager;
};

template <typename T>
using ComponentPtr = std::shared_ptr<T>;
template <typename T>
using ComponentList = std::vector<std::shared_ptr<T>>;

class ComponentManager
{
private:
    std::array<std::vector<std::weak_ptr<Component>>, maxComponents> m_components;
    Entity *m_entity;

protected:
public:
    ComponentManager() {}

    template <typename T>
    void registerComponent(std::shared_ptr<Component> &component)
    {
        m_components[getComponentID<T>()].push_back(component);
    }

    template <typename T>
    ComponentList<T> getComponents()
    {
        auto &a = m_components[getComponentID<T>()];
        ComponentList<T> list;
        list.reserve(a.size());
        for (auto &c : a)
        {
            list.push_back(std::dynamic_pointer_cast<T>(c.lock()));
        }
        return list;
    }

    template <typename T>
    void foreach (const std::function<void(T &)> &callback)
    {
        auto &a = m_components[getComponentID<T>()];
        for (auto &c : a)
        {
            callback(c);
        }
    }

    /** Initialize Component Manager */
    void init();
    template <typename T>
    void update(time_ds delta)
    {
        for (auto &c : m_components[getComponentID<T>()])
        {
            auto l = c.lock();
            if (l->enabled())
                l->update(delta);
        }
    }
    void refresh();
};

class Entity
{
private:
    std::array<std::shared_ptr<Component>, maxComponents> m_components;
    std::bitset<maxComponents> m_componentBitset;

    std::shared_ptr<Entity> m_parent;
    std::vector<std::shared_ptr<Entity>> m_children;

protected:
public:
    Entity() {}

    /** Check if entity has component T */
    template <typename T>
    bool hasComponent()
    {
        return m_componentBitset[getComponentID<T>()];
    }
    /** Removes component T attached to entity */
    template <typename T>
    void removeComponent()
    {
        m_components[getComponentID<T>()].reset();
    }
    /** Creates and allocate component T on the entity */
    template <typename T, typename... TArgs>
    T &addComponent(TArgs &&...args)
    {
        /** @todo: change return type to ComponentPtr<T> */
        
        T *c = nullptr;
        if (!hasComponent<T>()) {
            c = new T(std::forward<TArgs>(args)...);
            std::shared_ptr<Component> p(c);

            m_components[getComponentID<T>()] = p;
            m_componentBitset[getComponentID<T>()] = true;
            Game::componentManager().registerComponent<T>(p);
        }
        else {
            c = dynamic_cast<T*>(m_components[getComponentID<T>()].get());
        }

        c->m_entity = this;
        c->init();

        return *c;
    }
    /** Retrieves the component T of entity */
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
    virtual void fixedUpdate(time_ds delta) {}
    /** Called for every frame update (uses scaled time) */
    virtual void update(time_ds delta) {}
    /** Called after updating most components */
    virtual void postUpdate() {}
    /** Called once when entity is reused */
    virtual void reset() {}
    /** Called once before sending object back to be reused */
    virtual void clean() {}

    void parent(std::shared_ptr<Entity> &parent) { m_parent = parent; }
    std::shared_ptr<Entity> parent() { return m_parent; }

    void addChild(std::shared_ptr<Entity> &child) { m_children.push_back(child); }
    const std::vector<std::shared_ptr<Entity>> children() { return m_children; }

    friend EntityManager;
};

using EntityType = std::size_t;
template <typename T>
using EntityList = std::vector<std::shared_ptr<T>>;

class EntityManager
{
private:
    std::map<std::type_index, std::vector<std::shared_ptr<Entity>>> m_entities;

protected:
public:
    EntityManager() {}
    
    /** Creates and allocates a new entity object of type */
    template <typename T, typename... TArgs>
    T &addEntity(TArgs &&...args)
    {
        std::cout << "Creating entity for " << typeid(T).name() << " hash: " << std::type_index(typeid(T)).hash_code() << std::endl;
        T *e = new T(std::forward<TArgs>(args)...);
        std::shared_ptr<Entity> p(e);
        m_entities[std::type_index(typeid(T))].push_back(std::move(p));

        e->init();
        return *e;
    }

    template <typename T, typename... TArgs>
    EntityList<T> addEntities(const int &numEntities,
                              TArgs &&...args)
    {
        std::cout << "Creating entity for " << typeid(T).name() << " hash: " << std::type_index(typeid(T)).hash_code() << std::endl;
        std::vector<std::shared_ptr<T>> l_entities;
        l_entities.reserve(numEntities);

        for (int i = 0; i < numEntities; i++)
        {
            T *e = new T(std::forward<TArgs>(args)...);
            l_entities.emplace_back(e);
            std::shared_ptr<T> &p = l_entities[l_entities.size() - 1];
            m_entities[std::type_index(typeid(T))].push_back(p);

            e->init();
        }

        return l_entities;
    }

    template <typename T>
    void foreach (const std::function<void(T &)> &callback)
    {
        auto &a = m_entities[std::type_index(typeid(T))];
        for (auto &c : a)
        {
            callback(c);
        }
    }

    /** Initialize Entity Manager */
    void init();
    /** @todo: add threading for update methods? */
    /** Calls preUpdate for all entities */
    void preUpdate();
    /** Calls update for all entities */
    void update(time_ds delta);
    /** Calls postUpdate for all entities */
    void postUpdate();

    void refresh();
};