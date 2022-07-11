#pragma once

#include <core/ecs/entity.hpp>
#include <core/ecs/components.hpp>

class Enemy : public Entity
{
private:
    TransformComponent *m_transform;

    bool m_isActive;
protected:
public:
    Enemy();
    ~Enemy();

    void spawn(int type, const glm::vec3 &position);

    /** Entity overrides */
    void init() override;
    void preUpdate() override;
    void update(time_ms delta) override;
    void postUpdate() override;
    // void reset() override;
    // void clean() override;
};