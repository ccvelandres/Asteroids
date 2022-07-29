#include <core/graphics/camera/camera.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <core/utils/logging.hpp>

Camera::Camera(const float &width, const float &height, Projection mode)
    : m_position(glm::vec3(0.0f, 0.0f, -1.0f)),
      m_front(glm::vec3(0.0f, 0.0f, 1.0f)),
      m_projection(mode),
      m_screenSize(width, height),
      m_fieldOfView(glm::radians(90.0f)),
      m_aspectRatio(4.0f / 3.0f),
      m_nearClippingPlane(0.1f),
      m_farClippingPlane(100.0f),
      m_up(glm::vec3(0.0f, -1.0f, 0.0f))
{
    L_TAG("Camera::Camera");

    /** Update matrix view on creation */
    updateMatrix();

    L_TRACE("Internal resources initialized ({})", static_cast<void *>(this));
}

void Camera::updatePerspectiveMatrix()
{
    m_projectionMatrix = glm::perspective(m_fieldOfView, m_aspectRatio, m_nearClippingPlane, m_farClippingPlane);
    m_viewMatrix       = glm::lookAt(m_position, m_position + m_front, m_up);
}

void Camera::updateOrthogonalMatrix()
{
    glm::vec2 cameraBounds = glm::vec2(m_position) - (m_screenSize * 0.5f);
    m_projectionMatrix =
        glm::ortho(cameraBounds.x, cameraBounds.x + m_screenSize.x, cameraBounds.y, cameraBounds.y + m_screenSize.y);
    m_viewMatrix = glm::lookAt(m_position, m_position + m_front, m_up);
}

void Camera::updateMatrix()
{
    L_TAG("Camera::updateMatrix");

    switch (this->m_projection)
    {
    case Projection::Perspective:
        updatePerspectiveMatrix();
        break;
    case Projection::Orthographic:
        updateOrthogonalMatrix();
        break;
    default:
        L_THROW_RUNTIME("Invalid projection mode");
        break;
    }
}

void Camera::translate(const glm::vec3 &v) { m_position += v; }

void Camera::rotate(float angle, const glm::vec3 &axis)
{
    m_position = glm::vec3(glm::rotate(glm::mat4(1.0f), glm::radians(angle), axis) * glm::vec4(m_position, 1.0f));
}

Camera &Camera::setPosition(const glm::vec3 &pos) noexcept
{
    m_position = pos;
    return *this;
}

Camera &Camera::setDirection(const glm::vec3 &dir) noexcept
{
    m_front = dir;
    return *this;
}

Camera &Camera::setDirection(const glm::vec2 &angle) noexcept
{
    constexpr glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3           tFront  = glm::vec3(cos(angle.y) * sin(angle.x), sin(angle.y), cos(angle.y) * cos(angle.x));
    glm::vec3           right   = glm::cross(worldUp, tFront);
    glm::vec3           tUp     = glm::cross(right, tFront);

    m_front = glm::normalize(tFront);
    m_up    = glm::normalize(tUp);
    return *this;
}

Camera &Camera::setProjection(const Projection m) noexcept
{
    m_projection = m;
    return *this;
}

Camera &Camera::setScreenSize(const glm::vec2 &screenSize) noexcept
{
    m_screenSize = screenSize;
    return *this;
}

Camera &Camera::setFoV(const float deg) noexcept
{
    m_fieldOfView = glm::radians(deg);
    return *this;
}

Camera &Camera::setAspectRatio(const float asp) noexcept
{
    m_aspectRatio = asp;
    return *this;
}

Camera &Camera::setNearClippingPlane(const float clip) noexcept
{
    m_nearClippingPlane = clip;
    return *this;
}

Camera &Camera::setFarClippingPlane(const float clip) noexcept
{
    m_farClippingPlane = clip;
    return *this;
}

Camera &Camera::setRenderMask(const RenderMask renderMask) noexcept
{
    m_renderMask = renderMask;
    return *this;
}

const glm::mat4         &Camera::getProjectionMatrix() const noexcept { return m_projectionMatrix; }
const glm::mat4         &Camera::getViewMatrix() const noexcept { return m_viewMatrix; }
const glm::vec3         &Camera::getPosition() const noexcept { return m_position; }
const glm::vec3         &Camera::getFront() const noexcept { return m_front; }
const Camera::Projection Camera::getProjection() const noexcept { return m_projection; }
const glm::vec2         &Camera::getScreenSize() const noexcept { return m_screenSize; }
const float              Camera::getFieldOfView() const noexcept { return m_fieldOfView; }
const float              Camera::getAspectRatio() const noexcept { return m_aspectRatio; }
const float              Camera::getNearClippingPlane() const noexcept { return m_nearClippingPlane; }
const float              Camera::getFarClippingPlane() const noexcept { return m_farClippingPlane; }
const glm::vec3         &Camera::getUp() const noexcept { return m_up; }
const RenderMask        &Camera::getRenderMask() const noexcept { return m_renderMask; }