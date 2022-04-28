#pragma once

#include <game.hpp>
#include <ecs/ecs.hpp>
#include <ecs/components.hpp>

class Bullet : public Entity
{
private:
protected:
    TransformComponent *m_transform;
    SpriteComponent *m_sprite;
    RenderComponent *m_render;

    Entity *m_parent;
    int m_bulletID;

    Vector3F m_velocity;
    bool m_isShot;
    unsigned int m_shootTime;
public:
    Bullet(Entity &parent);
    ~Bullet();

    void shoot(const Vector3F &position,
               const Vector3F &velocity);
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