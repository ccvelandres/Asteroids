#include <core/ecs/components/renderComponent.hpp>
#include <core/ecs/entity.hpp>

RenderComponent::RenderComponent() = default;
RenderComponent::~RenderComponent() = default;

void RenderComponent::init() {
    this->m_transform = &this->m_entity->getComponent<TransformComponent>();
}