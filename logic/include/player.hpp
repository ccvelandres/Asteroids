#pragma once

#include <game.hpp>
#include <ecs/ecs.hpp>
#include <input/inputManager.hpp>
#include <ecs/components.hpp>

#include <bullet.hpp>

class Player : public Entity
{
private:
    TransformComponent *m_transform;

    EntityList<Bullet> m_bullets;
    int                speed;
    time_ds            shootInterval;
    time_ds            m_lastShoot;
protected:
public:
    Player();
    ~Player();

    /** Entity overrides */
    void init() override;
    void preUpdate() override;
    void update(time_ms delta) override;
    void postUpdate() override;
    // void reset() override;
    // void clean() override;
};