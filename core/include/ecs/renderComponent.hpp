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

    /** Component overrides */
    // void awake() override;
    void init() override;
    // void update(time_ms delta) override;
    // void clean() override;

    void render();

};