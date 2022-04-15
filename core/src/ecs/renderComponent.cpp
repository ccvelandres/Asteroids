#include <game.hpp>
#include <ecs/renderComponent.hpp>

void RenderComponent::init()
{
    sprite = &this->entity().getComponent<SpriteComponent>();
    transform = &this->entity().getComponent<TransformComponent>();
}

void RenderComponent::render()
{
    Renderer &r = Game::renderer();

    r.drawTexture2D(sprite->texture(),
                    transform->position.x,
                    transform->position.y,
                    transform->scale.x,
                    transform->scale.y);
}
