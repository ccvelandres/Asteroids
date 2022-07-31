#include <core/graphics/camera/camera.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <core/utils/logging.hpp>

constexpr glm::vec3 worldFront = glm::vec3(0.0f, 0.0f, 1.0f);
constexpr glm::vec3 worldRight = glm::vec3(1.0f, 0.0f, 0.0f);
constexpr glm::vec3 worldUp    = glm::vec3(0.0f, 1.0f, 0.0f);

Camera::Camera(const float &width, const float &height, Projection mode)
    : m_position(glm::vec3(0.0f, 0.0f, -1.0f)),
      m_orientation(glm::vec3(0.0f, 0.0f, 0.0f)),
      m_projection(mode),
      m_screenSize(width, height),
      m_fieldOfView(glm::radians(90.0f)),
      m_aspectRatio(4.0f / 3.0f),
      m_nearClippingPlane(0.1f),
      m_farClippingPlane(100.0f)
{
    L_TAG("Camera::Camera");

    /** Update matrix view on creation */
    updateMatrix();

    L_TRACE("Internal resources initialized ({})", static_cast<void *>(this));
}

void Camera::updatePerspectiveMatrix()
{
    glm::vec3 front = m_orientation * worldFront;
    glm::vec3 up    = m_orientation * worldUp;

    m_projectionMatrix = glm::perspective(m_fieldOfView, m_aspectRatio, m_nearClippingPlane, m_farClippingPlane);
    m_viewMatrix       = glm::lookAt(m_position, front, up);
}

void Camera::updateOrthogonalMatrix()
{
    glm::vec3 front = m_orientation * worldFront;
    glm::vec3 up    = m_orientation * worldUp;

    /** screen size defines the xy of bounding box
     * while the clipping planes defines the z ranges of the bounding box
     */

    glm::vec2 cameraBounds = glm::vec2(m_position) - (m_screenSize * 0.5f);
    m_projectionMatrix     = glm::ortho(cameraBounds.x,
                                    cameraBounds.x + m_screenSize.x,
                                    cameraBounds.y,
                                    cameraBounds.y + m_screenSize.y,
                                    m_nearClippingPlane,
                                    m_farClippingPlane);
    m_viewMatrix           = glm::lookAt(m_position, front, up);
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

void Camera::translate(const glm::vec3 &v) noexcept { this->m_position = this->m_position + v; }

void Camera::rotate(const glm::vec3 &e, bool relativeToSelf) noexcept
{
    glm::quat rotationQuat = glm::quat(glm::radians(e));
    this->m_orientation    = relativeToSelf ? this->m_orientation * rotationQuat : rotationQuat * this->m_orientation;
}

void Camera::rotate(float angle, const glm::vec3 &v, bool relativeToSelf) noexcept
{
    glm::quat rotationQuat = glm::angleAxis(glm::radians(angle), glm::normalize(v));
    this->m_orientation    = relativeToSelf ? this->m_orientation * rotationQuat : rotationQuat * this->m_orientation;
}

void Camera::rotateYXZ(const glm::vec3 &v, bool relativeToSelf) noexcept
{
    glm::quat rotationQuat = glm::quat(glm::radians(glm::vec3(v.y, v.x, v.z)));
    this->m_orientation    = relativeToSelf ? this->m_orientation * rotationQuat : rotationQuat * this->m_orientation;
}

Camera &Camera::setPosition(const glm::vec3 &pos) noexcept
{
    m_position = pos;
    return *this;
}

Camera &Camera::setOrientation(const glm::quat &q) noexcept
{
    this->m_orientation = q;
    return *this;
}

Camera &Camera::setOrientation(const glm::vec3 &forward, const glm::vec3 &up) noexcept
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

Camera &Camera::setOrientationEuler(const glm::vec3 &v) noexcept
{
    glm::quat orientation = glm::quat(glm::radians(v));
    this->m_orientation   = orientation;
    return *this;
}

Camera &Camera::setOrientationEulerYXZ(const glm::vec3 &v) noexcept
{
    glm::quat orientation = glm::quat(glm::radians(glm::vec3(v.y, v.x, v.z)));
    this->m_orientation   = orientation;
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
const glm::quat         &Camera::getOrientation() const noexcept { return m_orientation; }
const glm::vec3          Camera::getFront() const noexcept { return (m_orientation * worldFront); }
const glm::vec3          Camera::getUp() const noexcept { return (m_orientation * worldUp); }
const Camera::Projection Camera::getProjection() const noexcept { return m_projection; }
const glm::vec2         &Camera::getScreenSize() const noexcept { return m_screenSize; }
const float              Camera::getFieldOfView() const noexcept { return m_fieldOfView; }
const float              Camera::getAspectRatio() const noexcept { return m_aspectRatio; }
const float              Camera::getNearClippingPlane() const noexcept { return m_nearClippingPlane; }
const float              Camera::getFarClippingPlane() const noexcept { return m_farClippingPlane; }
const RenderMask        &Camera::getRenderMask() const noexcept { return m_renderMask; }