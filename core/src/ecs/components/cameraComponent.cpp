#include <core/ecs/components/cameraComponent.hpp>
#include <core/ecs/entity.hpp>

#include <core/utils/logging.hpp>

constexpr glm::mat4 identityMatrix = glm::mat4(1.0f);

CameraComponent::CameraComponent()  = default;
CameraComponent::~CameraComponent() = default;

CameraComponent::CameraComponent(const glm::vec2 &viewportSize, const Projection &projection)
    : m_viewportSize(viewportSize),
      m_projection(projection),
      m_fieldOfView_r(glm::radians(90.0f)),
      m_aspectRatio(4.0f / 3.0f),
      m_nearClippingPlane(0.1f),
      m_farClippingPlane(100.0f),
      m_renderMask(0xFFFF)
{
}

void CameraComponent::init()
{
    if (this->m_entity->hasComponent<TransformComponent>())
    {
        this->m_transform = &this->m_entity->getComponent<TransformComponent>();
    }
    else
    {
        this->m_transform = &this->m_entity->addComponent<TransformComponent>();
    }
}

void CameraComponent::updatePerspectiveMatrix()
{
    m_projectionMatrix = glm::perspective(m_fieldOfView_r, m_aspectRatio, m_nearClippingPlane, m_farClippingPlane);
}

void CameraComponent::updateOrthogonalMatrix()
{
    /** screen size defines the xy of bounding box
     * while the clipping planes defines the z ranges of the bounding box
     */

    glm::vec2 cameraBounds = glm::vec2(m_transform->getPosition()) - (m_viewportSize * 0.5f);
    m_projectionMatrix     = glm::ortho(cameraBounds.x,
                                    cameraBounds.x + m_viewportSize.x,
                                    cameraBounds.y,
                                    cameraBounds.y + m_viewportSize.y,
                                    m_nearClippingPlane,
                                    m_farClippingPlane);
}

void CameraComponent::updateMatrix()
{
    L_TAG("CameraComponent::updateMatrix");

    switch (this->m_projection)
    {
    case Projection::Orthographic:
        this->updateOrthogonalMatrix();
        break;
    case Projection::Perspective:
        this->updatePerspectiveMatrix();
        break;
    default:
        L_THROW_RUNTIME("Unhandled camera projection mode: {}", this->m_projection);
    }
    /** @todo: this needs fixing together with transformComponent local space translation */
    this->m_viewMatrix = glm::mat4_cast(m_transform->getOrientation())
                       * glm::inverse(glm::translate(identityMatrix, m_transform->getPosition()));
}

CameraComponent &CameraComponent::setProjection(const Projection &projection) noexcept
{
    m_projection = projection;
    return *this;
}

CameraComponent &CameraComponent::setViewportSize(const glm::vec2 &viewportSize) noexcept
{
    m_viewportSize = viewportSize;
    return *this;
}

CameraComponent &CameraComponent::setFoV(const float &fov) noexcept
{
    m_fieldOfView_r = glm::radians(fov);
    return *this;
}

CameraComponent &CameraComponent::setFoV_r(const float &fov) noexcept
{
    m_fieldOfView_r = fov;
    return *this;
}

CameraComponent &CameraComponent::setAspectRatio(const float &aspectRatio) noexcept
{
    m_aspectRatio = aspectRatio;
    return *this;
}

CameraComponent &CameraComponent::setNearClippingPlane(const float &clip) noexcept
{
    m_nearClippingPlane = clip;
    return *this;
}

CameraComponent &CameraComponent::setFarClippingPlane(const float &clip) noexcept
{
    m_farClippingPlane = clip;
    return *this;
}

CameraComponent &CameraComponent::setRenderMask(const RenderMask &renderMask) noexcept
{
    m_renderMask = renderMask;
    return *this;
}

const glm::mat4 &CameraComponent::getProjectionMatrix() const noexcept { return this->m_projectionMatrix; }
const glm::mat4 &CameraComponent::getViewMatrix() const noexcept { return this->m_viewMatrix; }
const CameraComponent::Projection CameraComponent::getProjection() const noexcept { return this->m_projection; }
const glm::vec2                  &CameraComponent::getViewportSize() const noexcept { return this->m_viewportSize; }
const float       CameraComponent::getFieldOfView() const noexcept { return glm::degrees(this->m_fieldOfView_r); }
const float       CameraComponent::getFieldOfView_r() const noexcept { return this->m_fieldOfView_r; }
const float       CameraComponent::getAspectRatio() const noexcept { return this->m_aspectRatio; }
const float       CameraComponent::getNearClippingPlane() const noexcept { return this->m_nearClippingPlane; }
const float       CameraComponent::getFarClippingPlane() const noexcept { return this->m_farClippingPlane; }
const RenderMask &CameraComponent::getRenderMask() const noexcept { return this->m_renderMask; }