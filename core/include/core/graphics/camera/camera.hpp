#pragma once

/**
 * @file core/camera/camera.hpp
 * @author Cedric Velandres (ccvelandres@gmail.com)
 *
 * @addtogroup Camera
 * @ingroup Graphics
 * @{
 */

#include <glm/glm.hpp>

#include "../../ecs/components/renderComponent.hpp"

/**
 * @brief Base Class for Camera types
 * @todo: is it a good idea to make the camera an entity?
 */
class Camera
{
public:
    enum class Projection
    {
        Perspective,
        Orthographic
    };

private:
    void updatePerspectiveMatrix();
    void updateOrthogonalMatrix();

    glm::mat4 m_projectionMatrix; /** the projection matrix */
    glm::mat4 m_viewMatrix;       /** the view matrix */
protected:
public:
    glm::vec3  m_position;          /** the position of the camera */
    glm::quat  m_orientation;       /** orientation of the camera */
    Projection m_projection;        /** projection mode of camera */
    glm::vec2  m_screenSize;        /** screen size to use in orthogonal mode */
    float      m_fieldOfView;       /** field of View in perspective mode (radians) */
    float      m_aspectRatio;       /** aspect ratio in perspective mode */
    float      m_nearClippingPlane; /** near clipping plane in perspective mode */
    float      m_farClippingPlane;  /** far clipping plane in perspective mode */
    RenderMask m_renderMask;        /** defines which renderComponents can be rendered with this camera */

    Camera(const float &width, const float &height, Projection mode);

    Camera()                      = delete;
    virtual ~Camera()             = default;
    Camera(Camera &o)             = default;
    Camera &operator=(Camera &o)  = default;
    Camera(Camera &&o)            = default;
    Camera &operator=(Camera &&o) = default;

    /**
     * @brief Translates the camera position with given vector
     *
     * @param v translation vector
     */
    void translate(const glm::vec3 &v) noexcept;
    /**
     * @brief Rotate the camera with the given euler angle
     *
     * @param e rotation in euler angles
     * @param relativeToSelf if true, performs the rotation with respect to local axis, else
     * performs the rotation with respect to world axis
     */
    void rotate(const glm::vec3 &e, bool relativeToSelf = true) noexcept;
    /**
     * @brief Rotates the camera @p angle degrees along the axis
     *
     * @param angle rotation angle in degrees
     * @param ax rotation axis
     * @param relativeToSelf if true, performs the rotation with respect to local axis, else
     * performs the rotation with respect to world axis
     */
    void rotate(float angle, const glm::vec3 &v, bool relativeToSelf = true) noexcept;
    /**
     * @brief Rotate the camera with the given euler angle
     *
     * @param v euler angles in yaw-pitch-roll (YXZ) (degrees)
     */
    void rotateYXZ(const glm::vec3 &v, bool relativeToSelf = true) noexcept;

    /**
     * @brief Updates the camera matrices. Should be called after updating components to update the matrices
     */
    void updateMatrix();

    /** Set position of camera */
    Camera &setPosition(const glm::vec3 &pos) noexcept;
    /** Set orientation of the camera using quaternions */
    Camera &setOrientation(const glm::quat &q) noexcept;
    /** Set orientation of the camera such that the camera is looking to v */
    Camera &setOrientation(const glm::vec3 &forward, const glm::vec3 &up) noexcept;
    /** Set orientation of the camera using euler angles (degrees) in pitch-yaw-roll (XYZ) */
    Camera &setOrientationEuler(const glm::vec3 &v) noexcept;
    /** Set orientation of the camera using euler angles (degrees) in yaw-pitch-roll (YXZ) */
    Camera &setOrientationEulerYXZ(const glm::vec3 &v) noexcept;
    /** Set projection mode for the camera */
    Camera &setProjection(const Projection m) noexcept;
    /** Set screensize of the camera */
    Camera &setScreenSize(const glm::vec2 &screenSize) noexcept;
    /** Set field of view of camera (degrees) */
    Camera &setFoV(const float deg) noexcept;
    /** Set aspect ratio of camera */
    Camera &setAspectRatio(const float asp) noexcept;
    /** Set near clipping plane (orthographic: clip < 0, perspective: clip > 0) */
    Camera &setNearClippingPlane(const float clip) noexcept;
    /** Set near clipping plane */
    Camera &setFarClippingPlane(const float clip) noexcept;
    /** Set near clipping plane */
    Camera &setRenderMask(const RenderMask renderMask) noexcept;

    const glm::mat4  &getProjectionMatrix() const noexcept;
    const glm::mat4  &getViewMatrix() const noexcept;
    const glm::vec3  &getPosition() const noexcept;
    const glm::quat  &getOrientation() const noexcept;
    const glm::vec3   getFront() const noexcept;
    const glm::vec3   getUp() const noexcept;
    const Projection  getProjection() const noexcept;
    const glm::vec2  &getScreenSize() const noexcept;
    const float       getFieldOfView() const noexcept;
    const float       getAspectRatio() const noexcept;
    const float       getNearClippingPlane() const noexcept;
    const float       getFarClippingPlane() const noexcept;
    const RenderMask &getRenderMask() const noexcept;
};

/** @} endgroup Camera */
