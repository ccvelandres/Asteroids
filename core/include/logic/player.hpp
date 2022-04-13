#pragma once

#include <game.hpp>
#include <ecs/ecs.hpp>
#include <input/inputManager.hpp>
#include <ecs/components.hpp>

class Player : public Entity
{
private:
    TransformComponent *transform;
    SpriteComponent *sprite;
    RenderComponent *render;

protected:
public:
    Player()
    {
    }

    void init() override
    {
        transform = &this->addComponent<TransformComponent>();
        sprite = &this->addComponent<SpriteComponent>();
        render = &this->addComponent<RenderComponent>();

        // sprite->addSpriteSet(0, "assets/ship.png", 16, 24, 5, 2,
        //                   {60, 60, 60, 60, 60, 60, 60, 60, 60, 60});
        sprite->appendSpriteSet(0, "assets/ship.png", 16, 24, 0, 0, 60);
        sprite->appendSpriteSet(0, "assets/ship.png", 16, 24, 0, 24, 60);
        sprite->appendSpriteSet(1, "assets/ship.png", 16, 24, 16, 0, 60);
        sprite->appendSpriteSet(1, "assets/ship.png", 16, 24, 16, 24, 60);
        sprite->appendSpriteSet(2, "assets/ship.png", 16, 24, 32, 0, 60);
        sprite->appendSpriteSet(2, "assets/ship.png", 16, 24, 32, 24, 60);
        sprite->appendSpriteSet(3, "assets/ship.png", 16, 24, 48, 0, 60);
        sprite->appendSpriteSet(3, "assets/ship.png", 16, 24, 48, 24, 60);
        sprite->appendSpriteSet(4, "assets/ship.png", 16, 24, 54, 0, 60);
        sprite->appendSpriteSet(4, "assets/ship.png", 16, 24, 54, 24, 60);
    

    void update(float delta) override
    {
        if (Game::inputManager().isPressed(SDL_SCANCODE_UP))
        {
            transform->posY -= 1;
        }
        if (Game::inputManager().isPressed(SDL_SCANCODE_DOWN))
        {
            transform->posY += 1;
        }
        if (Game::inputManager().isPressed(SDL_SCANCODE_LEFT))
        {
            transform->posX -= 1;
        }
        if (Game::inputManager().isPressed(SDL_SCANCODE_RIGHT))
        {
            transform->posX += 1;
        }

        std::cout << "Player@ " << transform->posX << "," << transform->posY << std::endl;
    }
};