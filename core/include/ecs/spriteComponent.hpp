#pragma once

#include "ecs.hpp"
#include <game.hpp>
#include <renderer.hpp>

class SpriteComponent : public Component
{
protected:
    Texture texture;

public:
    SpriteComponent(const std::string &textureName) : texture(Game::renderer().loadTexture(textureName)) {}

    void init() override {
        
    }
};