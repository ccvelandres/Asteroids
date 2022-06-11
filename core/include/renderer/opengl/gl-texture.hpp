#pragma once

#include <memory>
#include "gl-wrapper.hpp"

#include <assets/texture.hpp>

class OpenGLTexture
{
private:
    struct Internal;
    std::unique_ptr<Internal> m_internal;
protected:
public:
    OpenGLTexture(const assets::Texture &mesh);
    OpenGLTexture(OpenGLTexture&&o);
    OpenGLTexture &operator=(OpenGLTexture&&o);
    ~OpenGLTexture();

    GLuint getVertexBufferId() const;
    GLuint getIndiceBufferId() const;
};

