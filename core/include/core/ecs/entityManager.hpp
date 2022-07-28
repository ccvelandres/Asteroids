#pragma once

/**
 * @file core/ecs/entityManager.hpp
 * @author Cedric Velandres (ccvelandres@gmail.com)
 * 
 * @addtogroup ECS
 * @{
 */

#include "entity.hpp"
#include "componentManager.hpp"
#include "../time.hpp"
#include "../utils/logging.hpp"

#include <typeindex>
#include <memory>
#include <map>
#include <vector>
#include <functional>

/**
 * @brief Contains the Base Entity Class for the ECS System
 *
 */

class EntityManager
{
private:
    std::map<EntityID, std::vector<EntityPtr<Entity>>> m_entities;
    static EntityManager                              *m_instance;

    /** Disable all constructors */
    EntityManager();
    EntityManager(EntityManager &o)             = delete;
    EntityManager(EntityManager &&o)            = delete;
    EntityManager &operator=(EntityManager &o)  = delete;
    EntityManager &operator=(EntityManager &&o) = delete;

    /**
     * @brief Registers @p entity to list of entities with matching @p id
     *
     * @param id EntityID retrieved with @ref getEntityID<T>()
     * @param entity entity to register
     */
    void registerEntity(EntityID id, Entity *const entity);
protected:
public:
    ~EntityManager();

    /**
     * @brief Get the Instance object
     *
     * @return EntityManager& reference to EntityManager
     */
    static EntityManager &getInstance();

    /**
     * @brief Creates and allocates entity of type @p T
     *
     * @tparam T type of entity derived from @ref Entity
     * @tparam TArgs parameter list for entity constructor
     * @param args arguments forwarded to entity constructor
     * @return T& reference to created entity
     */
    template <typename T, typename... TArgs>
    T &addEntity(TArgs &&...args)
    {
        L_TAG("addEntity");

        T *e = new T(std::forward<TArgs>(args)...);
        L_DEBUG("{}, addr({}) hash(0x{:x})", typeid(T).name(), static_cast<void *>(e), typeid(T).hash_code());
        registerEntity(getEntityID<T>(), e);

        e->init();
        return *e;
    }

    /**
     * @brief Creates and allocates @p numEntities count of entities with type @p T
     *
     * @tparam T type of entity derived from @ref Entity
     * @tparam TArgs parameter list for entity constructor
     * @param numEntities number of entities to create
     * @param args arguments forwarded to entity constructor
     * @return EntityList<T> vector of pointers to created entity
     */
    template <typename T, typename... TArgs>
    EntityList<T> addEntities(const int numEntities, TArgs &&...args)
    {
        L_TAG("addEntities");

        L_DEBUG("{}, count({}), hash(0x{:x})", typeid(T).name(), numEntities, typeid(T).hash_code());
        EntityList<T> l_entities;
        l_entities.reserve(numEntities);

        for (int i = 0; i < numEntities; i++)
            l_entities.push_back(&addEntity<T>(std::forward<TArgs>(args)...));

        return l_entities;
    }

    /**
     * @brief Applies the function @p callback to all entities of type @p T
     *
     * @tparam T type of entity
     * @param callback function to call for each entity
     */
    template <typename T>
    void foreach (const std::function<void(T &)> &callback)
    {
        auto &a = m_entities[getEntityID<T>()];
        for (auto &c : a)
        {
            /** @todo: maybe build vector of shared_ptrs first */
            callback(dynamic_cast<T &>(*c.get()));
        }
    }

    /** Initialize Entity Manager */
    void init();
    /** @todo: add threading for update methods? */
    /** Calls preUpdate for all entities */
    void preUpdate();
    /** Calls update for all entities */
    void update(time_ms delta);
    /** Calls postUpdate for all entities */
    void postUpdate();

    /** Deletes inactive entities */
    void refresh();
};

/** @} endgroup ECS */