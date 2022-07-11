#pragma once

/**
 * @addtogroup ECS
 * @{
 */

#include "common.hpp"
#include "component.hpp"

#include <memory>
#include <array>
#include <vector>
#include <functional>

/**
 * @brief Manages all types of Component
 *
 */
class ComponentManager
{
private:
    std::array<std::vector<std::weak_ptr<Component>>, maxComponents> m_components;
protected:
public:
    ComponentManager();

    /**
     * @brief Register @p component to the list of components of type @p T
     *
     * @tparam T typename of @p component
     * @param component component to register
     */
    template <typename T>
    void registerComponent(std::shared_ptr<Component> &component)
    {
        m_components[getComponentID<T>()].push_back(component);
    }

    /**
     * @brief Retrieves all components of type @p T
     *
     * @tparam T type of component
     * @return ComponentList<T> list of components of type @p T
     */
    template <typename T>
    ComponentList<T> getComponents()
    {
        auto            &a = m_components[getComponentID<T>()];
        ComponentList<T> list;
        list.reserve(a.size());
        for (auto &c : a)
        {
            list.push_back(std::dynamic_pointer_cast<T>(c.lock()));
        }
        return list;
    }

    /**
     * @brief Applies the function @p callback to all components of type @p T
     *
     * @tparam T type of component
     * @param callback function to call for each component
     */
    template <typename T>
    void foreach (const std::function<void(T &)> &callback)
    {
        auto &a = m_components[getComponentID<T>()];
        for (auto &c : a)
        {
            /** @todo: maybe build vector of shared_ptrs first */
            auto p = c.lock();
            callback(dynamic_cast<T &>(*p.get()));
        }
    }

    // /** Does initialization outside of the constructor */
    // void init();
    // /** Called for every frame update (uses unscaled time) */
    // void fixedUpdate(time_ms delta);
    // /** Called for every frame update (uses scaled time) */
    // void update(time_ms delta);
    // /** Called when the component is released */
    // void clean();
    /** Refreshes the component index */
    void refresh();
};

/** @} endgroup ECS */