#include <core/ecs/components/renderComponent.hpp>
#include <core/ecs/entity.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/quaternion.hpp>

RenderComponent::RenderComponent() = default;
RenderComponent::~RenderComponent() = default;

const glm::mat4 RenderComponent::getModelMatrix() noexcept
{
    TransformComponent &transform = m_entity->getComponent<TransformComponent>();

    /** @todo: minimize model matrix computation by computing only on change */
    const glm::mat4 identity(1.0f);
    glm::mat4       translation = glm::translate(identity, transform.m_position);
    glm::mat4       rotation    = glm::mat4_cast(transform.m_orientation);
    glm::mat4       scale       = glm::scale(identity, transform.m_scale);
    m_modelMatrix = translation * rotation * scale;
    return m_modelMatrix;
}