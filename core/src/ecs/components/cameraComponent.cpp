#include <core/ecs/components/cameraComponent.hpp>
#include <core/ecs/entity.hpp>
#include <core/game.hpp>

#include <core/utils/logging.hpp>

std::ostream& operator<<(std::ostream& os, const CameraComponent::Projection& p) {
    if( p == CameraComponent::Projection::Perspective ) 
        return os << "Perspective";
    if( p == CameraComponent::Projection::Orthographic ) 
        return os << "Orthographic";
    return os;
}

template <> struct fmt::formatter<CameraComponent::Projection> : ostream_formatter {};

constexpr glm::mat4 identityMatrix = glm::mat4(1.0f);

CameraComponent::CameraComponent()  = default;
CameraComponent::~CameraComponent() = default;

CameraComponent::CameraComponent(const Projection &projection)
    : m_projection(projection),
      m_viewportSize(Game::this_game()->getWindowSize()),
      m_fieldOfView_r(glm::radians(90.0f)),
      m_aspectRatio(m_viewportSize.x / m_viewportSize.y),
      m_nearClippingPlane(0.1f),
      m_farClippingPlane(100.0f),
      m_zscaling(0.01f),
      m_renderMask(0xFFFF)
{
}

CameraComponent::CameraComponent(const Projection &projection, const glm::vec2 &viewportSize)
    : m_projection(projection),
      m_viewportSize(viewportSize),
      m_fieldOfView_r(glm::radians(90.0f)),
      m_aspectRatio(m_viewportSize.x / m_viewportSize.y),
      m_nearClippingPlane(0.1f),
      m_farClippingPlane(100.0f),
      m_zscaling(0.01f),
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
    glm::vec2 screenCenter = glm::vec2(m_transform->getPosition());
    glm::vec2 screenSize   = m_viewportSize * m_zscaling * 0.5f;
    m_projectionMatrix = glm::ortho(screenCenter.x - screenSize.x,
                                    screenCenter.x + screenSize.x,
                                    screenCenter.y - screenSize.y,
                                    screenCenter.y + screenSize.y,
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