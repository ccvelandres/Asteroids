#include <cassert>

#include <SDL2/SDL_image.h>

#include <renderer/common/renderer.hpp>
#include <utils/logging.hpp>

Texture2D::Texture2D(std::shared_ptr<SDL_Texture> texture) : m_texture(texture)
{
    SDL_QueryTexture(m_texture.get(),
                     &m_pixelFormat,
                     &m_textureAccess,
                     &(m_size.x),
                     &(m_size.y));
}

Texture2D::Texture2D(std::shared_ptr<SDL_Texture> texture,
                     const Vector2 &position,
                     const Vector2 &size) : m_texture(texture),
                                            m_position(position),
                                            m_size(size)
{
    int w, h;
    SDL_QueryTexture(m_texture.get(), &m_pixelFormat, &m_textureAccess, &w, &h);
    assert(m_size.x >= m_position.x + size.x);
    assert(m_size.y >= m_position.y + size.y);
}

std::shared_ptr<SDL_Texture> Texture2D::openTexture(SDL_Renderer *const renderer,
                                                    const std::string &filename)
{
    L_TAG("Texture2D::openTexture");

    SDL_Texture *t = IMG_LoadTexture(renderer, filename.c_str());
    if (t == NULL)
    {
        L_ERROR("Could not open {}", filename);
    }
    else
    {
        L_DEBUG("Texture loaded from {} ({})",
                       filename,
                       fmt::ptr(t));
        std::shared_ptr<SDL_Texture> p(t, Texture2D::deallocTexture);
        return p;
    }
    return {};
}

void Texture2D::deallocTexture(SDL_Texture *tex)
{
    L_TAG("Texture2D::deallocTexture");
    
    L_DEBUG("Texture unload ({})", fmt::ptr(tex));
    SDL_DestroyTexture(tex);
}
