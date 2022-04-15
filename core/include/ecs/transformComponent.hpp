#pragma once

#include "ecs.hpp"
#include <utils/vector.hpp>

class TransformComponent : public Component
{
public:
    TransformComponent() : position(),
                           scale(1),
                           rotation(0) {}
    TransformComponent(const float &posX,
                       const float &posY,
                       const float &posZ) : position(posX, posY, posZ),
                                            scale(1),
                                            rotation(0) {}
    
    /** Component overrides */
    // void awake() override;
    // void init() override;
    // void update(time_ds delta) override;
    // void clean() override;

    Vector3F position;
    Vector3F scale;
    float rotation;
};