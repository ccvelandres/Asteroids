#pragma once

/**
 * @addtogroup Camera
 * @{
 */

#include <glm/glm.hpp>

/**
 * @file This file defines the base Camera class
 * @todo: maybe change to CRTP? do we need derived classes?
 */

/**
 * @brief Base Class for Camera types
 * @todo
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

    glm::mat4  m_projectionMatrix;  /** the projection matrix */
    glm::mat4  m_viewMatrix;        /** the view matrix */
    glm::vec3  m_up;                /** the up vector for the camera */
protected:
public:
    glm::vec3  m_position;          /** the position of the camera */
    glm::vec3  m_front;             /** direction vector where the camera is facing (unit vector) */
    Projection m_projection;        /** projection mode of camera */
    glm::vec2  m_screenSize;        /** screen size to use in orthogonal mode */
    float      m_fieldOfView;       /** field of View in perspective mode (radians) */
    float      m_aspectRatio;       /** aspect ratio in perspective mode */
    float      m_nearClippingPlane; /** near clipping plane in perspective mode */
    float      m_farClippingPlane;  /** far clipping plane in perspective mode */

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
    void translate(const glm::vec3 &v);

    /**
     * @brief Rotates the camera @p angle degrees along the axis
     *
     * @param angle rotation angle in degrees
     * @param ax rotation axis
     */
    void rotate(float angle, const glm::vec3 &ax);

    /**
     * @brief Updates the camera matrices. Should be called after updating components to update the matrices
     */
    void updateMatrix();

    /** Set position of camera */
    Camera &setPosition(const glm::vec3 &pos) noexcept;
    /** Set the direction of camera (unit vector) */
    Camera &setDirection(const glm::vec3 &dir) noexcept;
    /** Set the direction of camera (euler angle) */
    Camera &setDirection(const glm::vec2 &angle) noexcept;
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

    const glm::mat4 &getProjectionMatrix() const noexcept;
    const glm::mat4 &getViewMatrix() const noexcept;
    const glm::vec3 &getPosition() const noexcept;
    const glm::vec3 &getFront() const noexcept;
    const Projection getProjection() const noexcept;
    const glm::vec2 &getScreenSize() const noexcept;
    const float      getFieldOfView() const noexcept;
    const float      getAspectRatio() const noexcept;
    const float      getNearClippingPlane() const noexcept;
    const float      getFarClippingPlane() const noexcept;
    const glm::vec3 &getUp() const noexcept;
};

/** @} endgroup Camera */
