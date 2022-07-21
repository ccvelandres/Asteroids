#pragma once

/**
 * @file core/renderer/opengl/gl-mesh.hpp
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
    
    OpenGLMesh();
protected:
public:
    /**
     * @brief Creates a vertex buffer object from given mesh object
     *
     * @param mesh source mesh
     */
    OpenGLMesh(const assets::Mesh &mesh);

    ~OpenGLMesh();
    OpenGLMesh(OpenGLMesh &o)            = delete;
    OpenGLMesh &operator=(OpenGLMesh &o) = delete;
    OpenGLMesh(OpenGLMesh &&o);
    OpenGLMesh &operator=(OpenGLMesh &&o);

    /** Retrieve name of vertex array object */
    GLuint getVertexBufferID() const noexcept;
    /** Retrieve name of element array object */
    GLuint getIndiceBufferID() const noexcept;
    /** Retrieve count of indice (size of element buffer) */
    uint32_t getIndiceCount() const noexcept;
    /** Retrieve stride of vertex buffer object */
    GLsizei getStride() const noexcept;
    /** Retrieve offset of geometry vertex in vertex buffer object */
    GLsizei getOffsetPosition() const noexcept;
    /** Retrieve offset of vertex normals in vertex buffer object */
    GLsizei getOffsetNormals() const noexcept;
    /** Retrieve offset of uv coordinates in vertex buffer object */
    GLsizei getOffsetTexCoords() const noexcept;
};

/** @} endgroup OpenGL */
