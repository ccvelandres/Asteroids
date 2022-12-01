#include <core/ecs/component.hpp>

/**
 * @brief Contains the definition for the Base Component Class
 * 
 */

Component::Component() : m_enabled(true) {
    /** @todo: ECS-REWORK
     * Resolve the derived class component ID then
     * Register the component to the ComponentManager. 
     **/
}