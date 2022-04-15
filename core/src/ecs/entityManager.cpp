#include <ecs/ecs.hpp>

void EntityManager::preUpdate() {
    for (auto &e : m_entities){
        e->preUpdate();
    }
}

void EntityManager::update(time_ds delta) {
    for (auto &e : m_entities){
        e->update(delta);
    }
}

void EntityManager::postUpdate() {
    for (auto &e : m_entities){
        e->postUpdate();
    }
}

void EntityManager::refresh()
{
    m_entities.erase(std::remove_if(m_entities.begin(),
                                    m_entities.end(),
                                    [](const std::shared_ptr<Entity> &e)
                                    {
                                        return e->free();
                                    }),
                     m_entities.end());
}