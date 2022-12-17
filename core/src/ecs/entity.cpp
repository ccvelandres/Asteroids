#include <core/ecs/entity.hpp>
#include <core/ecs/entityManager.hpp>
#include <core/ecs/componentManager.hpp>
#include <core/game.hpp>

Entity::Entity()  = default;
Entity::~Entity() = default;

void Entity::addComponent(ComponentID id, ComponentPtr<Component> &component)
{
    m_components[id]      = component;
    m_componentBitset[id] = true;
    ComponentManager::getInstance().registerComponent(id, component);
}
