#pragma once

#include "ecs.hpp"
#include <glm/glm.hpp>

class TransformComponent : public Component
{
public:
    TransformComponent() : position(), scale(1), rotation(0) {}
    TransformComponent(const float &posX, const float &posY, const float &posZ)
        : position(posX, posY, posZ),
          scale(1),
          rotation(0)
    {
    }

    /** Component overrides */
    // void awake() override;
    // void init() override;
    // void update(time_ms delta) override;
    // void clean() override;

    glm::vec3 position;
    glm::vec3 scale;
    float     rotation;
};