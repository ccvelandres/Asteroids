#include <ecs/ecs.hpp>

void EntityManager::updateEntities(float delta) {
    for (auto &e : m_entities){
        e->update(delta);
    }
}

void EntityManager::refresh()
{
    m_entities.erase(std::remove_if(m_entities.begin(),
                                    m_entities.end(),
                                    [](const std::shared_ptr<Entity> &e)
                                    {
                                        return !e->active();
                                    }),
                     m_entities.end());
}