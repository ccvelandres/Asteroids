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

TransformComponent &TransformComponent::translate(const glm::vec3 &v, bool localSpace) noexcept
{
    if (localSpace)
    {
        /** net says translation of vector via quat is:
         * v` = quat * v * conjugate(quat)
         * however, the z axis is inversed. WHY?
         * this works the same -> v` = quat * v; BUT WHY?
         */
        glm::vec3 tV = glm::inverse(m_orientation) * v;
        this->m_position += tV;
    }
    else
    {
        // translate relative to world space
        this->m_position += v;
    }
    return *this;
}

TransformComponent &TransformComponent::scale(const glm::vec3 &v) noexcept
{
    this->m_scale = this->m_scale * v;
    return *this;
}

TransformComponent &TransformComponent::rotate(const glm::vec2 &e, bool localSpace) noexcept
{
    // rotation via spherical coordinates
    /** @todo: needs verification if this is correct */
    glm::vec2 v            = glm::radians(e);
    glm::quat rotationQuat = glm::quat(glm::vec3(-v.y, v.x, 0.0f));
    this->m_orientation    = localSpace ? this->m_orientation * rotationQuat : rotationQuat * this->m_orientation;
    return *this;
}

TransformComponent &TransformComponent::rotate(const glm::vec3 &euler, bool localSpace) noexcept
{
    // rotation via euler angles in XYZ - pitch-yaw-roll
    /** @todo: needs verification if this is correct */
    glm::quat rotationQuat = glm::quat(glm::radians(euler));
    this->m_orientation    = localSpace ? this->m_orientation * rotationQuat : rotationQuat * this->m_orientation;
    return *this;
}

TransformComponent &TransformComponent::rotateYXZ(const glm::vec3 &euler, bool localSpace) noexcept
{
    // rotation via euler angles in YXZ - yaw-pitch-roll
    /** @todo: needs verification if this is correct */
    glm::quat rotationQuat = glm::quat(glm::radians(glm::vec3(euler.y, euler.x, euler.z)));
    this->m_orientation    = localSpace ? this->m_orientation * rotationQuat : rotationQuat * this->m_orientation;
    return *this;
}

TransformComponent &TransformComponent::rotate(float angle, const glm::vec3 &axis, bool localSpace) noexcept
{
    // rotation via angle-axis
    glm::quat rotationQuat = glm::angleAxis(glm::radians(angle), glm::normalize(axis));
    this->m_orientation    = localSpace ? this->m_orientation * rotationQuat : rotationQuat * this->m_orientation;
    return *this;
}

/** Setters */

TransformComponent &TransformComponent::setPosition(const glm::vec3 &v) noexcept
{
    this->m_position = v;
    return *this;
}

TransformComponent &TransformComponent::setScale(const glm::vec3 &v) noexcept
{
    this->m_scale = v;
    return *this;
}

TransformComponent &TransformComponent::setOrientation(const glm::quat &q) noexcept
{
    this->m_orientation = q;
    return *this;
}

TransformComponent &TransformComponent::setOrientation(const glm::vec3 &forward, const glm::vec3 &up) noexcept
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

TransformComponent &TransformComponent::setOrientationEuler(const glm::vec3 &v) noexcept
{
    this->m_orientation   = glm::quat(glm::radians(v));
    return *this;
}

TransformComponent &TransformComponent::setOrientationEulerYXZ(const glm::vec3 &v) noexcept
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

const glm::vec3 TransformComponent::getFront() const noexcept
{
    return TransformComponent::worldFront * this->m_orientation;
}

const glm::vec3 TransformComponent::getRight() const noexcept
{
    return TransformComponent::worldRight * this->m_orientation;
}

const glm::vec3 TransformComponent::getUp() const noexcept
{
    return TransformComponent::worldUp * this->m_orientation;
}