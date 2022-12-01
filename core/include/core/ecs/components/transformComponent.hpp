#pragma once

/**
 * @file core/ecs/components/transformComponent.hpp
 * @author Cedric Velandres (ccvelandres@gmail.com)
 *
 * @addtogroup Components
 * @{
 */

#include "../component.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

/**
 * @brief The TransformComponent describes the position, scale, orientation of
 * attached entities.
 *
 * In 2D Rendering mode, XY acts as normal and Z acts as depth buffering
 *
 * @todo: parent links with transform and world space resolution
 */
class TransformComponent : public Component
{
private:
protected:
    /** Protected Constructors (use entity to add components) */
    TransformComponent(glm::vec3 position, glm::vec3 scale, glm::vec3 rotation);
    TransformComponent();

public:
    static constexpr glm::vec3 worldFront = glm::vec3(0.0f, 0.0f, 1.0f);
    static constexpr glm::vec3 worldUp    = glm::vec3(0.0f, 1.0f, 0.0f);
    static constexpr glm::vec3 worldRight = glm::vec3(1.0f, 0.0f, 0.0f);

    glm::vec3 m_position;    /** the position of the object in world space */
    glm::vec3 m_scale;       /** the scale of the object */
    glm::quat m_orientation; /** the orientation of the object */

    ~TransformComponent();
    TransformComponent(TransformComponent &o)             = delete;
    TransformComponent &operator=(TransformComponent &o)  = delete;
    TransformComponent(TransformComponent &&o)            = default;
    TransformComponent &operator=(TransformComponent &&o) = default;

    /** Component overrides */
    // void awake() override;
    // void init() override;
    // void update(time_ms delta) override;
    // void clean() override;

    /**
     * @brief Apply position translation to the object
     *
     * @param v translation vector
     * @param localSpace if true, translation is done in the object's local space,
     * else the translation is relative to world space
     */
    TransformComponent &translate(const glm::vec3 &v, bool localSpace = false) noexcept;
    /**
     * @brief Apply scale to the object
     *
     * @param v scale vector
     */
    TransformComponent &scale(const glm::vec3 &v) noexcept;
    /**
     * @brief Rotate the entity using two angles in spherical coordinates. Useful for rotating
     * the entity in first-person view.
     * Rotations are represented as:
     *  - x -> rotation along y axis with clockwise as positive
     *  - y -> rotation along x axis with clockwise as positive
     *
     * @param e rotation in degrees angle (vertical,horizontal)
     * @param localSpace if true, rotation is done in the object's local space, else
     * rotation is done in  world space
     * @return TransformComponent& reference to this component
     */
    TransformComponent &rotate(const glm::vec2 &e, bool localSpace = true) noexcept;
    /**
     * @brief Rotate the object with the given euler angle.
     * Rotations are interpreted as:
     *  - x -> rotation along x axis with clockwise as positive value
     *  - y -> rotation along y axis with clockwise as positive value
     *  - z -> rotation along z axis with clockwise as positive value
     *
     * @param euler euler angles in pitch-yaw-roll (XYZ) (degrees)
     * @param localSpace if true, rotation is done in the object's local space, else
     * rotation is done in  world space
     * @return TransformComponent& reference to this camera object
     */
    TransformComponent &rotate(const glm::vec3 &euler, bool localSpace = true) noexcept;
    /**
     * @brief Rotate the object with the given euler angle.
     * Rotations are interpreted as:
     *  - x -> rotation along y axis with clockwise as positive value
     *  - y -> rotation along x axis with clockwise as positive value
     *  - z -> rotation along z axis with clockwise as positive value
     *
     * @param euler euler angles in yaw-pitch-roll (YXZ) (degrees)
     * @param localSpace if true, rotation is done in the object's local space, else
     * rotation is done in  world space
     * @return Camera& reference to this camera object
     */
    TransformComponent &rotateYXZ(const glm::vec3 &euler, bool localSpace = true) noexcept;
    /**
     * @brief Rotate the object with @p angle along @p axis
     *
     * @param angle angle of rotation (degrees)
     * @param axis axis of rotation
     * @param localSpace if true, rotation is done in the object's local space, else
     * rotation is done in  world space
     */
    TransformComponent &rotate(float angle, const glm::vec3 &axis, bool localSpace = true) noexcept;

    /** @todo: should we use method chaining for setters? */
    /** Set position of the object */
    TransformComponent &setPosition(const glm::vec3 &v) noexcept;
    /** Set scale of the object */
    TransformComponent &setScale(const glm::vec3 &v) noexcept;
    /** Set orientation of the object using quaternions */
    TransformComponent &setOrientation(const glm::quat &q) noexcept;
    /** Set orientation of the object such that it is looking to v */
    TransformComponent &setOrientation(const glm::vec3 &forward,
                                       const glm::vec3 &up = TransformComponent::worldUp) noexcept;
    /** Set orientation of the object using euler angles (degrees) in pitch-yaw-roll (XYZ) */
    TransformComponent &setOrientationEuler(const glm::vec3 &euler) noexcept;
    /** Set orientation of the object using euler angles (degrees) in yaw-pitch-roll (YXZ) */
    TransformComponent &setOrientationEulerYXZ(const glm::vec3 &euler) noexcept;

    /** Retrieve the the object's current position */
    const glm::vec3 getPosition() const noexcept;
    /** Retrieve the the object's current scale */
    const glm::vec3 getScale() const noexcept;
    /** Retrieve the the object's current orientation */
    const glm::quat getOrientation() const noexcept;
    /** Retrieve the the object's current orientation (degrees) in pitch-yaw-roll (XYZ) */
    const glm::vec3 getOrientationXYZ() const noexcept;
    /** Retrieve the the object's current orientation (degrees) in yaw-pitch-roll (YXZ) */
    const glm::vec3 getOrientationYXZ() const noexcept;
    /** Retrieve the the object's front vector */
    const glm::vec3 getFront() const noexcept;
    /** Retrieve the the object's right vector */
    const glm::vec3 getRight() const noexcept;
    /** Retrieve the the object's up vector */
    const glm::vec3 getUp() const noexcept;

    void setParent(TransformComponent *t) noexcept;

    friend Entity;
    friend EntityManager;
    friend ComponentManager;
};

/** @} endgroup Components */