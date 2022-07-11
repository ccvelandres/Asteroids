#include <core/ecs/componentManager.hpp>

ComponentManager::ComponentManager() = default;

void ComponentManager::refresh()
{
    for (auto &v : m_components)
    {
        v.erase(std::remove_if(v.begin(), v.end(), [](const std::weak_ptr<Component> &p) { return p.expired(); }),
                v.end());
    }
}