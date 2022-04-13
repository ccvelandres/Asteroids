#pragma once

#include "ecs.hpp"

class TransformComponent : public Component
{
public:
    TransformComponent() : posX(0), posY(0){}
    TransformComponent(const float &posX,
                       const float &posY) : posX(posX),
                                            posY(posY) {}

    float posX, posY;
};