#include <core/camera/perspective-camera.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/matrix.hpp>

#include <core/utils/logging.hpp>

static glm::mat4 createProjectionMatrix(const float &width, const float &height)
{
    return glm::perspective(glm::radians(60.0f), width / height, 0.01f, 100.0f);
}

static glm::mat4 createViewMatrix()
{
    glm::vec3 position(0.0f, 0.0f, 2.0f);
    glm::vec3 target(0.0f, 0.0f, 0.0f);
    glm::vec3 up(0.0f, 1.0f, 0.0f);

    return glm::lookAt(position, target, up);
}

PerspectiveCamera::PerspectiveCamera(const float &width, const float &height)
    : m_projectionMatrix(::createProjectionMatrix(width, height)),
      m_viewMatrix(::createViewMatrix())
{
    L_TAG("PerspectiveCamera::PerspectiveCamera");
    L_TRACE("Internal resources initialized ({})", static_cast<void *>(this));
}

PerspectiveCamera::PerspectiveCamera()  = default;
PerspectiveCamera::~PerspectiveCamera() = default;

glm::mat4 &PerspectiveCamera::getProjectionMatrix() { return m_projectionMatrix; }
glm::mat4 &PerspectiveCamera::getViewMatrix() { return m_viewMatrix; }