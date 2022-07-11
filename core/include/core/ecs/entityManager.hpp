#pragma once

/**
 * @addtogroup ECS
 * @{
 */

#include "common.hpp"
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
    std::map<std::type_index, std::vector<std::unique_ptr<Entity>>> m_entities;

protected:
public:
    EntityManager() {}

    /** Creates and allocates a new entity object of type */

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
        std::unique_ptr<Entity> p(e);
        m_entities[std::type_index(typeid(T))].push_back(std::move(p));

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
        {
            T *e = new T(std::forward<TArgs>(args)...);
            l_entities.emplace_back(e);
            std::unique_ptr<T> p(e);
            m_entities[std::type_index(typeid(T))].push_back(std::move(p));

            e->init();
        }

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
        auto &a = m_entities[std::type_index(typeid(T))];
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