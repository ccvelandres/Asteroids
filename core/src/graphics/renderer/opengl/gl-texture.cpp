#include <core/graphics/renderer/opengl/gl-texture.hpp>
#include <utils/logging.hpp>

#include "SDL2/SDL_opengl.h"

#include <vector>

/**
 * @brief Flips the surface vertically. Needed for OpenGL use
 * 
 * @param surface surface to flip
 */
static void flipv_surface(SDL_Surface *surface) noexcept {
    L_TAG("createTexture");

    // Lock surface if needed
    if (SDL_MUSTLOCK(surface)) SDL_LockSurface(surface);

    char *pixels = reinterpret_cast<char*>(surface->pixels);
    int pitch = surface->pitch; // row size
    int rows = surface->h;
    char *rowBuffer = new char[pitch]; // row buffer

    /** basically each loop we swap the equivalent row from top and bottom
     *  first row becomes the last row, ... till we reach the middle
     * 
     */
    for(int i = 0; i < rows / 2; i++){
        char *r1 = pixels + (i * pitch); // top row
        char *r2 = pixels + ((rows - i - 1) * pitch); // bottom row

        memcpy(rowBuffer, r1, pitch);
        memcpy(r1, r2, pitch);
        memcpy(r2, rowBuffer, pitch);
    }

    // Unlock surface if needed
    if (SDL_MUSTLOCK(surface)) SDL_UnlockSurface(surface);
}

static GLuint createTexture(const core::assets::Texture &texture)
{
    L_TAG("createTexture");

    GLuint       textureId;
    SDL_Surface *surface = texture.getSurface();

    // Flip the texture first before loading
    flipv_surface(surface);

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

    Internal(const core::assets::Texture &texture) : textureID(createTexture(texture))
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

OpenGLTexture::OpenGLTexture(const core::assets::Texture &texture) : m_internal(std::make_unique<Internal>(texture)) {}

OpenGLTexture::OpenGLTexture()                             = default;
OpenGLTexture::OpenGLTexture(OpenGLTexture &&o)            = default;
OpenGLTexture &OpenGLTexture::operator=(OpenGLTexture &&o) = default;
OpenGLTexture::~OpenGLTexture()                            = default;

GLuint OpenGLTexture::getTextureID() const { return m_internal->textureID; }