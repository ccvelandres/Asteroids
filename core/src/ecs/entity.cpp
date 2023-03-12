#include <core/ecs/entity.hpp>
#include <core/ecs/entityManager.hpp>
#include <core/ecs/componentManager.hpp>
#include <core/game.hpp>

Entity::Entity()  = default;
Entity::~Entity() = default;

void Entity::addComponent(const ComponentID &id, ComponentPtr<Component> &component)
{
    this->m_components.emplace(id, component);
    ComponentManager::getInstance().registerComponent(id, component);
}

bool Entity::hasComponent(const ComponentID &id) const noexcept
{
    return this->m_components.find(id) == this->m_components.end() ? false : true;
}

void Entity::removeComponent(const ComponentID &id)
{
    auto it = this->m_components.find(id);
    if (it != this->m_components.end())
    {
        it->second.reset();
    }
}

ComponentPtr<Component> Entity::getComponent(const ComponentID &id) const
{
    L_TAG("Entity::getComponent");

    auto it = this->m_components.find(id);
    if (it == this->m_components.end())
    {
        L_THROW_RUNTIME("Entity {} has no component {}:0x{:x}",
                        static_cast<const void *>(this),
                        id.name(),
                        id.hash_code());
    }

    return it->second;
}
