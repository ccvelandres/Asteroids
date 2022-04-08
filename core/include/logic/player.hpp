#pragma once

#include <ecs/ecs.hpp>
#include <ecs/components.hpp>

class Player : public Entity {
private:
    TransformComponent *transform;
    SpriteComponent *sprite;
protected:
public:
    Player() {
        transform = &this->addComponent<TransformComponent>();
        sprite = &this->addComponent<SpriteComponent>("assets/ship.png");
    }

    void init() {

    }

    void update() {
        
    }
};