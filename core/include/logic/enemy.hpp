#pragma once

#include <ecs/ecs.hpp>
#include <ecs/components.hpp>


class Enemy : public Entity {
private:
    TransformComponent *m_transform;

    bool m_isActive;
protected:
public:
    Enemy();
    ~Enemy();

    void spawn(int type, const Vector3F &position);

    /** Entity overrides */
    void init() override;
    void preUpdate() override;
    void update(time_ms delta) override;
    void postUpdate() override;
    // void reset() override;
    // void clean() override;
};