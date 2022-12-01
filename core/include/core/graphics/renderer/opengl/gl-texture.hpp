#pragma once

/**
 * @file core/renderer/opengl/gl-texture.hpp
 * @author Cedric Velandres (ccvelandres@gmail.com)
 * 
 * @addtogroup OpenGL
 * @{
 */

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
    OpenGLTexture();
    OpenGLTexture(const core::assets::Texture &mesh);
    ~OpenGLTexture();

    OpenGLTexture(OpenGLTexture &o)            = delete;
    OpenGLTexture &operator=(OpenGLTexture &o) = delete;
    OpenGLTexture(OpenGLTexture &&o);
    OpenGLTexture &operator=(OpenGLTexture &&o);

    GLuint getTextureID() const;
};

/** @} endgroup OpenGL */
