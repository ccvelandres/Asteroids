#pragma once

/**
 * @file gl-mesh.hpp
 * @author Cedric Velandres (ccvelandres@gmail.com)
 */

/**
 * @addtogroup OpenGL
 * @ingroup Renderer
 * @{
 */

#include <memory>
#include "gl-wrapper.hpp"

#include <assets/mesh.hpp>

/**
 * @brief Allows storing mesh data for rendering
 * 
 */
class OpenGLMesh
{
private:
    struct Internal;
    std::unique_ptr<Internal> m_internal;
protected:
public:
    OpenGLMesh(const assets::Mesh &mesh);

    OpenGLMesh();
    ~OpenGLMesh();
    OpenGLMesh(OpenGLMesh &o)            = delete;
    OpenGLMesh &operator=(OpenGLMesh &o) = delete;
    OpenGLMesh(OpenGLMesh &&o);
    OpenGLMesh &operator=(OpenGLMesh &&o);

    GLuint   getVertexBufferId() const;
    GLuint   getIndiceBufferId() const;
    uint32_t getIndiceCount() const;
};

/** @} endgroup OpenGL */
