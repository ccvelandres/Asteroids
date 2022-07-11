#include <core/ecs/entityManager.hpp>
#include <core/utils/logging.hpp>

void EntityManager::preUpdate()
{
    for (auto &[hash, vector] : m_entities)
    {
        for (auto &e : vector)
        {
            e->preUpdate();
        }
    }
}

void EntityManager::update(time_ms delta)
{
    for (auto &[hash, vector] : m_entities)
    {
        for (auto &e : vector)
        {
            e->update(delta);
        }
    }
}

void EntityManager::postUpdate()
{
    for (auto &[hash, vector] : m_entities)
    {
        for (auto &e : vector)
        {
            e->postUpdate();
        }
    }
}

void EntityManager::refresh()
{
    for (auto &v : m_entities)
        v.second.erase(std::remove_if(v.second.begin(),
                                      v.second.end(),
                                      [](const std::unique_ptr<Entity> &e) { return !e->isActive; }),
                       v.second.end());
}