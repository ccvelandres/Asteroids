#include <core/ecs/components/transformComponent.hpp>
#include <core/utils/logging.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/quaternion.hpp>

TransformComponent::TransformComponent() : m_position(0.0f), m_scale(1.0f), m_orientation(glm::vec3(0.0f)) {}
TransformComponent::TransformComponent(glm::vec3 position, glm::vec3 scale, glm::vec3 rotation)
    : m_position(position),
      m_scale(scale),
      m_orientation(glm::vec3(0.0f))
{
}

TransformComponent::~TransformComponent() = default;

void TransformComponent::translate(const glm::vec3 &v) noexcept { this->m_position += v; }

void TransformComponent::scale(const glm::vec3 &v) noexcept { this->m_scale *= v; }

void TransformComponent::rotate(float angle, const glm::vec3 &v) noexcept
{
    this->m_orientation *= glm::angleAxis(angle, v);
}

void TransformComponent::rotateXYZ(const glm::vec3 &v) noexcept { this->m_orientation *= glm::quat(v); }

void TransformComponent::rotateYXZ(const glm::vec3 &v) noexcept
{
    this->m_orientation *= glm::quat(glm::vec3(v.y, v.x, v.z));
}

/** Setters */

const TransformComponent &TransformComponent::setPosition(const glm::vec3 &v) noexcept
{
    this->m_position = v;
    return *this;
}

const TransformComponent &TransformComponent::setScale(const glm::vec3 &v) noexcept
{
    this->m_scale = v;
    return *this;
}

const TransformComponent &TransformComponent::setOrientation(const glm::quat &q) noexcept
{
    this->m_orientation = q;
    return *this;
}

const TransformComponent &TransformComponent::setOrientationXYZ(const glm::vec3 &v) noexcept
{
    this->m_orientation = glm::quat(v);
    return *this;
}

const TransformComponent &TransformComponent::setOrientationYXZ(const glm::vec3 &v) noexcept
{
    this->m_orientation = glm::quat(glm::vec3(v.y, v.x, v.z));
    return *this;
}

/** Getters */

const glm::vec3 &TransformComponent::getPosition() const noexcept { return m_position; }

const glm::vec3 &TransformComponent::getScale() const noexcept { return m_scale; }

const glm::quat &TransformComponent::getOrientation() const noexcept { return m_orientation; }

const glm::vec3 TransformComponent::getOrientationXYZ() const noexcept { return glm::eulerAngles(m_orientation); }

const glm::vec3 TransformComponent::getOrientationYXZ() const noexcept
{
    glm::vec3 euler = glm::eulerAngles(m_orientation);
    return glm::vec3(euler.y, euler.x, euler.z);
}

const glm::mat4 TransformComponent::getModelMatrix() const noexcept
{
    /** @todo: minimize model matrix computation by computing only on change */
    const glm::mat4 identity(1.0f);
    glm::mat4       translation = glm::translate(identity, this->m_position);
    glm::mat4       rotation    = glm::mat4_cast(this->m_orientation);
    glm::mat4       scale       = glm::scale(identity, this->m_scale);
    return translation * rotation * scale;
}