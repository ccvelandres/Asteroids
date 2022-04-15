#pragma once

#include <ecs/ecs.hpp>
#include <ecs/components.hpp>


class Enemy : public Entity {
private:
    TransformComponent *m_transform;
    SpriteComponent *m_sprite;
    RenderComponent *m_render;

    bool m_isActive;
protected:
public:
    Enemy();
    ~Enemy();

    void spawn(int type, const Vector3F &position);

    /** Entity overrides */
    void init() override;
    void preUpdate() override;
    void update(time_ds delta) override;
    void postUpdate() override;
    // void reset() override;
    // void clean() override;
};