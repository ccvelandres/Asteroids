#pragma once

/**
 * @file core/ecs/components/cameraComponent.hpp
 * @author Cedric Velandres (ccvelandres@gmail.com)
 *
 * @addtogroup Components
 * @{
 */

#include "../component.hpp"
#include "transformComponent.hpp"
#include "renderComponent.hpp"

#include <bitset>

/**
 * @brief The CameraComponent controls which components are displayed to the screen
 *
 */
class CameraComponent : public Component
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

    TransformComponent *m_transform;         /** the camera's transform component */
    glm::mat4           m_projectionMatrix;  /** the projection matrix */
    glm::mat4           m_viewMatrix;        /** the view matrix */
    Projection          m_projection;        /** current projection mode of the camera */
    glm::vec2           m_viewportPosition;  /** viewport position */
    glm::vec2           m_viewportSize;      /** viewport size of the camera */
    float               m_fieldOfView_r;     /** field of view in perspective mode (radians) */
    float               m_aspectRatio;       /** aspect ratio in perspective mode */
    float               m_nearClippingPlane; /** near clipping plane in orthogonal mode */
    float               m_farClippingPlane;  /** far clipping plane in orthogonal mode */
    float               m_zscaling;          /** z-scaling factor for orthogonal view */
    RenderMask          m_renderMask;        /** controls which renderComponents are visible */

protected:
    /** Protected Constructors (use entity to add components) */
    CameraComponent();
    CameraComponent(const Projection &projection);
    CameraComponent(const Projection &projection, const glm::vec2 &viewportSize);
public:

    ~CameraComponent();
    CameraComponent(CameraComponent &o)             = delete;
    CameraComponent &operator=(CameraComponent &o)  = delete;
    CameraComponent(CameraComponent &&o)            = default;
    CameraComponent &operator=(CameraComponent &&o) = default;

    /** Component overrides */
    /** Called when the component is first created */
    void init() override;
    /** Called for every frame update (uses unscaled time) */
    // void fixedUpdate(time_ms delta) override;
    /** Called for every frame update (uses scaled time) */
    // void update(time_ms delta) override;
    /** Called when the component is released */
    // void clean() override;

    /**
     * @brief Update the camera matrices
     *
     */
    void updateMatrix();

    /** Set projection mode for the camera */
    CameraComponent &setProjection(const Projection &projection) noexcept;
    /** Set viewport position of the camera */
    CameraComponent &setViewportPosition(const glm::vec2 &viewportPosition) noexcept;
    /** Set screensize of the camera */
    CameraComponent &setViewportSize(const glm::vec2 &viewportSize) noexcept;
    /** Set field of view of camera (degrees) */
    CameraComponent &setFoV(const float &fov) noexcept;
    /** Set field of view of camera (radians) */
    CameraComponent &setFoV_r(const float &fov) noexcept;
    /** Set aspect ratio of camera */
    CameraComponent &setAspectRatio(const float &aspectRatio) noexcept;
    /** Set near clipping plane (orthographic: clip < 0, perspective: clip > 0) */
    CameraComponent &setNearClippingPlane(const float &clip) noexcept;
    /** Set near clipping plane */
    CameraComponent &setFarClippingPlane(const float &clip) noexcept;
    /** Set near clipping plane */
    CameraComponent &setRenderMask(const RenderMask &renderMask) noexcept;
    /** Set Z-scaling mode for orthogonal view */
    CameraComponent &setZScaling(const float &scale) noexcept;

    const glm::mat4  &getProjectionMatrix() const noexcept;
    const glm::mat4  &getViewMatrix() const noexcept;
    const Projection  getProjection() const noexcept;
    const glm::vec2  &getViewportPosition() const noexcept;
    const glm::vec2  &getViewportSize() const noexcept;
    const float       getFieldOfView() const noexcept;
    const float       getFieldOfView_r() const noexcept;
    const float       getAspectRatio() const noexcept;
    const float       getNearClippingPlane() const noexcept;
    const float       getFarClippingPlane() const noexcept;
    const RenderMask &getRenderMask() const noexcept;
    const float       getZScaling() const noexcept;

    friend Entity;
    friend EntityManager;
    friend ComponentManager;
};

/** @} endgroup Components */
