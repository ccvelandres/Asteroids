#include <game.hpp>
#include <ecs/renderComponent.hpp>

void RenderComponent::render()
{
    Renderer &r = Game::renderer();

    r.drawTexture(sprite->texture(),
                  transform->posX,
                  transform->posY);
}