#pragma once

#include <core/game.hpp>
#include <core/ecs/entity.hpp>
#include <core/ecs/components.hpp>

class Bullet : public Entity
{
private:
protected:
    TransformComponent *m_transform;

    Entity *m_parent;
    int           m_bulletID;

    glm::vec3 m_velocity;
    bool      m_isShot;
    time_ms   m_shootTime;
public:
    Bullet(Entity *parent);
    ~Bullet();

    void shoot(const glm::vec3 &position, const glm::vec3 &velocity);
    void onHit();

    bool isShot() { return m_isShot; }

    /** Entity overrides */
    void init() override;
    void preUpdate() override;
    void update(time_ms delta) override;
    void postUpdate() override;
    // void reset() override;
    // void clean() override;
};