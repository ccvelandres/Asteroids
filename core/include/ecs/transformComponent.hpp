#pragma once

#include "ecs.hpp"

class TransformComponent : public Component
{
protected:
    float posX, posY;

public:
    TransformComponent() {}
    TransformComponent(const float &posX,
                       const float &posY) : posX(posX),
                                            posY(posY) {}
};