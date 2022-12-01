#pragma once

/**
 * @file core/renderer/opengl/gl-mesh.hpp
 * @author Cedric Velandres (ccvelandres@gmail.com)
 *
 * @addtogroup OpenGL
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
public:
    struct VertexInfo
    {
        GLuint   vertexBufferID;
        GLuint   indiceBufferID;
        uint32_t indiceCount;
        GLsizei  stride;
        GLsizei  offsetPosition;
        GLsizei  offsetNormals;
        GLsizei  offsetTexCoords;
    };
private:
    VertexInfo m_vertexInfo;
    OpenGLMesh();
protected:
public:
    /**
     * @brief Creates a vertex buffer object from given mesh object
     *
     * @param mesh source mesh
     */
    OpenGLMesh(const core::assets::Mesh &mesh);

    ~OpenGLMesh();
    OpenGLMesh(OpenGLMesh &o)            = delete;
    OpenGLMesh &operator=(OpenGLMesh &o) = delete;
    OpenGLMesh(OpenGLMesh &&o);
    OpenGLMesh &operator=(OpenGLMesh &&o);

    /** Retrieve name of vertex array object */
    const GLuint &getVertexBufferID() const noexcept;
    /** Retrieve name of element array object */
    const GLuint &getIndiceBufferID() const noexcept;
    /** Retrieve count of indice (size of element buffer) */
    const uint32_t &getIndiceCount() const noexcept;
    /** Retrieve stride of vertex buffer object */
    const GLsizei &getStride() const noexcept;
    /** Retrieve offset of geometry vertex in vertex buffer object */
    const GLsizei &getOffsetPosition() const noexcept;
    /** Retrieve offset of vertex normals in vertex buffer object */
    const GLsizei &getOffsetNormals() const noexcept;
    /** Retrieve offset of uv coordinates in vertex buffer object */
    const GLsizei &getOffsetTexCoords() const noexcept;
    /** Retrieve vertex info for the the mesh object */
    const VertexInfo &getVertexInfo() const noexcept;
};

/** @} endgroup OpenGL */
