#include <core/graphics/renderer/opengl/gl-texture.hpp>
#include <utils/logging.hpp>

#include "SDL2/SDL_opengl.h"

#include <vector>

GLuint createTexture(const assets::Texture &texture)
{
    L_TAG("createTexture");

    GLuint       textureId;
    SDL_Surface *surface = texture.getSurface();

    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RGBA,
                 static_cast<GLsizei>(surface->w),
                 static_cast<GLsizei>(surface->h),
                 0,
                 GL_RGBA,
                 GL_UNSIGNED_BYTE,
                 surface->pixels);
    glGenerateMipmap(GL_TEXTURE_2D);

    L_DEBUG("GLTexture loaded to {}", textureId);

    return textureId;
}

struct OpenGLTexture::Internal
{
    const GLuint textureID;

    Internal(const assets::Texture &texture) : textureID(createTexture(texture))
    {
        L_TAG("OpenGLTexture::Internal");
        L_TRACE("Internal resources initialized ({})", static_cast<void *>(this));
    }

    ~Internal()
    {
        L_TAG("OpenGLTexture::~Internal");
        L_TRACE("Internal resources freed ({})", static_cast<void *>(this));
    }
};

OpenGLTexture::OpenGLTexture(const assets::Texture &texture) : m_internal(std::make_unique<Internal>(texture)) {}

OpenGLTexture::OpenGLTexture()                             = default;
OpenGLTexture::OpenGLTexture(OpenGLTexture &&o)            = default;
OpenGLTexture &OpenGLTexture::operator=(OpenGLTexture &&o) = default;
OpenGLTexture::~OpenGLTexture()                            = default;
