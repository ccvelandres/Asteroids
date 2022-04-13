#pragma once

#include <ecs/ecs.hpp>
#include <ecs/transformComponent.hpp>
#include <ecs/spriteComponent.hpp>
#include <renderer.hpp>

class RenderComponent : public Component {
    SpriteComponent *sprite;
    TransformComponent *transform;
public:
    RenderComponent () {}

    void init () override {
        sprite = &this->getEntity().getComponent<SpriteComponent>();
        transform = &this->getEntity().getComponent<TransformComponent>();
    }

    void update (float delta) override {
        
    }

    void render();
};