#pragma once

/**
 * @addtogroup Camera
 * @{
 */

#include <glm/glm.hpp>

class PerspectiveCamera
{
private:
    glm::mat4 m_projectionMatrix;
    glm::mat4 m_viewMatrix;
protected:
public:
    PerspectiveCamera(const float &width, const float &height);

    PerspectiveCamera();
    ~PerspectiveCamera();
    PerspectiveCamera(PerspectiveCamera &o)             = default;
    PerspectiveCamera &operator=(PerspectiveCamera &o)  = default;
    PerspectiveCamera(PerspectiveCamera &&o)            = default;
    PerspectiveCamera &operator=(PerspectiveCamera &&o) = default;

    glm::mat4 &getProjectionMatrix();
    glm::mat4 &getViewMatrix();
};

/** @} endgroup Camera */