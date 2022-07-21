#pragma once

/**
 * @file core/ecs/components/transformComponent.hpp
 * @author Cedric Velandres (ccvelandres@gmail.com)
 */

/**
 * @addtogroup Components
 * @ingroup ECS
 * @{
 */

#include "../component.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

/**
 * @brief The Transform Component describes the position, scale, orientation of 
 * attached entities
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
    glm::vec3 m_position;    /** the position of attached entity in world space */
    glm::vec3 m_scale;       /** the scale of attached entity */
    glm::quat m_orientation; /** the orientation of attached entity */
    
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
     * @brief Apply position translation to attached entity
     * 
     * @param v translation vector
     */
    void translate(const glm::vec3 &v) noexcept;
    /**
     * @brief Apply scale to attached entity 
     * 
     * @param v scale vector
     */
    void scale(const glm::vec3 &v) noexcept;
    /**
     * @brief Rotate attached entity with @p angle along axis @p r
     * 
     * @param angle angle of rotation (radians)
     * @param r axis of rotation
     */
    void rotate(float angle, const glm::vec3 &v) noexcept;
    /**
     * @brief Rotate attached entity using euler angles in radians
     * 
     * @param v euler angles in pitch-yaw-roll (XYZ)
     */
    void rotateXYZ(const glm::vec3 &v) noexcept;
    /**
     * @brief Rotate attached entity using euler angles in radians
     * 
     * @param v euler angles in yaw-pitch-roll (YXZ)
     */
    void rotateYXZ(const glm::vec3 &v) noexcept;

    /** @todo: should we use method chaining for setters? */
    /** Set position of attached entity */
    const TransformComponent& setPosition(const glm::vec3 &v) noexcept;    
    /** Set scale of attached entity */
    const TransformComponent& setScale(const glm::vec3 &v) noexcept;
    /** Set orientation of attached entity using quaternions */
    const TransformComponent& setOrientation(const glm::quat &q) noexcept;
    /** Set orientation of attached entity using euler angles (radians) in pitch-yaw-roll (XYZ) */
    const TransformComponent& setOrientationXYZ(const glm::vec3 &v) noexcept;
    /** Set orientation of attached entity using euler angles (radians) in yaw-pitch-roll (YXZ) */
    const TransformComponent& setOrientationYXZ(const glm::vec3 &v) noexcept;

    /** Retrieve the attached entity's current position */
    const glm::vec3 &getPosition() const noexcept;
    /** Retrieve the attached entity's current scale */
    const glm::vec3 &getScale() const noexcept;
    /** Retrieve the attached entity's current orientation (radians) */
    const glm::quat &getOrientation() const noexcept;
    /** Retrieve the attached entity's current orientation (radians) in pitch-yaw-roll (XYZ) */
    const glm::vec3 getOrientationXYZ() const noexcept;
    /** Retrieve the attached entity's current orientation (radians) in yaw-pitch-roll (YXZ) */
    const glm::vec3 getOrientationYXZ() const noexcept;

    /** Retrieve the attached entity's model matrix (translation * rotation * scale) */
    const glm::mat4 getModelMatrix() const noexcept;

    void setParent(TransformComponent *t) noexcept;

    friend Entity;
    friend EntityManager;
    friend ComponentManager;
};

/** @} endgroup Components */