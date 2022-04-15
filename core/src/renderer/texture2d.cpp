#include <renderer.hpp>
#include <cassert>

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