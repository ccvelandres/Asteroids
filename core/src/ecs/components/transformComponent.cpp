#include <core/ecs/components/transformComponent.hpp>
#include <core/utils/logging.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/quaternion.hpp>

constexpr glm::vec3 worldFront = glm::vec3(0.0f, 0.0f, 1.0f);
constexpr glm::vec3 worldUp    = glm::vec3(0.0f, 1.0f, 0.0f);
constexpr glm::vec3 worldRight = glm::vec3(1.0f, 0.0f, 0.0f);

TransformComponent::TransformComponent() : m_position(0.0f), m_scale(1.0f), m_orientation(glm::vec3(0.0f)) {}
TransformComponent::TransformComponent(glm::vec3 position, glm::vec3 scale, glm::vec3 rotation)
    : m_position(position),
      m_scale(scale),
      m_orientation(glm::vec3(0.0f))
{
}

TransformComponent::~TransformComponent() = default;

glm::vec3 TransformComponent::front() const noexcept { return (worldFront * m_orientation); }
glm::vec3 TransformComponent::right() const noexcept { return (m_orientation * worldUp); }
glm::vec3 TransformComponent::up() const noexcept { return (m_orientation * worldRight); }

void TransformComponent::translate(const glm::vec3 &v) noexcept { this->m_position = this->m_position + v; }

void TransformComponent::scale(const glm::vec3 &v) noexcept { this->m_scale = this->m_scale * v; }

void TransformComponent::rotate(const glm::vec3 &e, bool relativeToSelf) noexcept
{
    glm::quat rotationQuat = glm::quat(glm::radians(e));
    this->m_orientation    = relativeToSelf ? this->m_orientation * rotationQuat : rotationQuat * this->m_orientation;
}

void TransformComponent::rotate(float angle, const glm::vec3 &v, bool relativeToSelf) noexcept
{
    glm::quat rotationQuat = glm::angleAxis(glm::radians(angle), glm::normalize(v));
    this->m_orientation    = relativeToSelf ? this->m_orientation * rotationQuat : rotationQuat * this->m_orientation;
}

void TransformComponent::rotateYXZ(const glm::vec3 &v, bool relativeToSelf) noexcept
{
    glm::quat rotationQuat = glm::quat(glm::radians(glm::vec3(v.y, v.x, v.z)));
    this->m_orientation    = relativeToSelf ? this->m_orientation * rotationQuat : rotationQuat * this->m_orientation;
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

const TransformComponent &TransformComponent::setOrientation(const glm::vec3 &forward, const glm::vec3 &up) noexcept
{
    /** @todo pretty sure this is incorrect. in short we want to be able to
     * set the orientation using a direction vector
     * or set the rotation axis of the quat to the direction vector
     * and we can leave the quat's rotation angle as 1 rad?
     *
     * or in shorter terms, we want to set direction such that we are looking
     * at the vector
     */
    glm::quat orientation = glm::conjugate(glm::quat(glm::lookAt(this->m_position, forward, up)));
    this->m_orientation   = orientation;
    return *this;
}

const TransformComponent &TransformComponent::setOrientationEuler(const glm::vec3 &v) noexcept
{
    glm::quat orientation = glm::quat(glm::radians(v));
    this->m_orientation   = orientation;
    return *this;
}

const TransformComponent &TransformComponent::setOrientationEulerYXZ(const glm::vec3 &v) noexcept
{
    glm::quat orientation = glm::quat(glm::radians(glm::vec3(v.y, v.x, v.z)));
    this->m_orientation   = orientation;
    return *this;
}

/** Getters */

const glm::vec3 TransformComponent::getPosition() const noexcept { return m_position; }

const glm::vec3 TransformComponent::getScale() const noexcept { return m_scale; }

const glm::quat TransformComponent::getOrientation() const noexcept { return m_orientation; }

const glm::vec3 TransformComponent::getOrientationXYZ() const noexcept
{
    glm::vec3 euler = glm::eulerAngles(m_orientation);
    return glm::degrees(euler);
}

const glm::vec3 TransformComponent::getOrientationYXZ() const noexcept
{
    glm::vec3 euler = glm::eulerAngles(m_orientation);
    return glm::degrees(glm::vec3(euler.y, euler.x, euler.z));
}