#pragma once

#include <core/game.hpp>
#include <core/ecs/entity.hpp>
#include <core/input/inputManager.hpp>
#include <core/ecs/components.hpp>

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