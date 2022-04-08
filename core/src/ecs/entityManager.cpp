#include <ecs/ecs.hpp>

template <typename T, typename... TArgs>
T &EntityManager::addEntity(const EntityType& type, TArgs &&...args)
{
    T *e = new T(std::forward<TArgs>(args)...);
    m_entities.push_back(std::make_shared(e)); // is this move?

    e->init();
}

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