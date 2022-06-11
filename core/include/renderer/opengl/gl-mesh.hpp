#pragma once

#include <memory>
#include "gl-wrapper.hpp"

#include <assets/mesh.hpp>

class OpenGLMesh
{
private:
    struct Internal;
    std::unique_ptr<Internal> m_internal;
protected:
public:
    OpenGLMesh(const assets::Mesh &mesh);
    OpenGLMesh(OpenGLMesh&&o);
    OpenGLMesh &operator=(OpenGLMesh&&o);
    ~OpenGLMesh();

    GLuint getVertexBufferId() const;
    GLuint getIndiceBufferId() const;
};

