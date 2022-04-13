#pragma once

#include <memory>
#include <vector>
#include <array>
#include <algorithm>
#include <bitset>

#include <game.hpp>

class Component;
class Entity;
class EntityManager;

using ComponentID = std::size_t;
constexpr std::size_t maxComponents = 32;

static ComponentID getComponentID()
{
    static ComponentID lastID = 0;
    return lastID++;
}

template <typename T>
static ComponentID getComponentID()
{
    static ComponentID cID = getComponentID();
    return cID;
}

class Component
{
private:
    Entity *m_entity;

protected:
    Entity &getEntity() { return *m_entity; }

public:
    Component() {};
    virtual ~Component() {};

    virtual void awake() {}
    virtual void init() {}
    virtual void update(float delta) {}
    virtual void clean() {}

    friend Entity;
    friend EntityManager;
};

template <typename T>
using ComponentPtr = std::shared_ptr<T>;
template<typename T>
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
        return std::move(list);
    }

    void init();
    template <typename T>
    void update(float delta)
    {
        for (auto &c : m_components[getComponentID<T>()])
        {
            auto l = c.lock();
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

    bool m_isActive, m_isFree;

protected:
public:
    Entity() : m_isActive(true),
               m_isFree(false) {}

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
        assert(hasComponent<T>() == false);

        T *c = new T(std::forward<TArgs>(args)...);
        std::shared_ptr<Component> p(c);

        m_components[getComponentID<T>()] = p;
        m_componentBitset[getComponentID<T>()] = true;
        Game::componentManager().registerComponent<T>(p);

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

        T *c =  dynamic_cast<T*>(m_components[getComponentID<T>()].get());
        return *c;
    }

    /** Called once on allocation of entity */
    virtual void init() {}
    /** Called once every frame update */
    virtual void update(float delta) {}
    /** Called once when entity is reused */
    virtual void reset() {}
    /** Called once before sending object back to be reused */
    virtual void clean() {}

    /** Calling this will free the entity on next refresh */
    void free() { m_isFree = true; }
    /** Get the active status of entity */
    bool active() { return m_isActive; }
    /** Set the active status of entity */
    void active(const bool &active) { m_isActive = active; }

    friend EntityManager;
};

using EntityType = std::size_t;

class EntityManager
{
private:
    std::vector<std::shared_ptr<Entity>> m_entities;

protected:
public:
    EntityManager() {}

    /** Creates and allocates a new entity object of type */
    template <typename T, typename... TArgs>
    T &addEntity(const EntityType &type = 0, TArgs &&...args)
    {
        T *e = new T(std::forward<TArgs>(args)...);
        std::shared_ptr<Entity> p(e);
        m_entities.push_back(std::move(p));

        e->init();
        return *e;
    }

    void updateEntities(float delta);

    void refresh();
};