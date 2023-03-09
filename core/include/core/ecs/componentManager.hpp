#pragma once

/**
 * @file core/ecs/componentManager.hpp
 * @author Cedric Velandres (ccvelandres@gmail.com)
 * 
 * @addtogroup ECS
 * @{
 */

#include "component.hpp"

#include <vector>
#include <unordered_map>
#include <functional>

/**
 * @brief Manages all types of Component
 *
 */
class ComponentManager
{
private:
    std::unordered_map<ComponentID, std::vector<ComponentWeakPtr<Component>>> m_components;
    static ComponentManager                                         *m_instance;

    /** Disable all constructors */
    ComponentManager();
    ComponentManager(ComponentManager &o)             = delete;
    ComponentManager(ComponentManager &&o)            = delete;
    ComponentManager &operator=(ComponentManager &o)  = delete;
    ComponentManager &operator=(ComponentManager &&o) = delete;

    /**
     * @brief Register @p component to list of component with matching @p id
     *
     * @param id ComponentID retrieved with @ref getComponentID<T>
     * @param component component to register
     */
    void registerComponent(ComponentID id, const ComponentPtr<Component> &component);
protected:
public:
    ~ComponentManager();

    /**
     * @brief Get the Instance object
     *
     * @return ComponentManager& reference to ComponentManager
     */
    static ComponentManager &getInstance();

    /**
     * @brief Register @p component to the list of components of type @p T
     *
     * @tparam T typename of @p component
     * @param component component to register
     */
    template <typename T, std::enable_if_t<std::is_base_of<Component, T>::value, bool> = true>
    void registerComponent(const ComponentPtr<T> &component)
    {
        registerComponent(getComponentID<T>(), component);
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
        for (auto &c : getComponents<T>()) callback((*c));
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

    friend Entity;
};

/** @} endgroup ECS */