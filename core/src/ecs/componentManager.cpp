#include <core/ecs/componentManager.hpp>

ComponentManager *ComponentManager::m_instance = nullptr;

ComponentManager::ComponentManager() = default;
ComponentManager::~ComponentManager() { m_instance = nullptr; }

ComponentManager &ComponentManager::getInstance()
{
    if (!m_instance) m_instance = new ComponentManager();
    return *m_instance;
}

void ComponentManager::registerComponent(ComponentID id, const ComponentPtr<Component> &component)
{
    m_components[id].push_back(component);
}

void ComponentManager::refresh()
{
    for (auto &v : m_components)
    {
        v.erase(std::remove_if(v.begin(), v.end(), [](const std::weak_ptr<Component> &p) { return p.expired(); }),
                v.end());
    }
}