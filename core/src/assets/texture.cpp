#include <assets/texture.hpp>

#include <utils/logging.hpp>

Texture::Texture( const std::string &path )
{
    L_TAG( "Texture::Texture" );
    m_surface = IMG_Load( path.c_str() );
    if ( m_surface == NULL )
    {
        L_THROW_RUNTIME( "Could not load texture {}", path );
    }
}

Texture::Texture( Texture &&o )            = default;
Texture &Texture::operator=( Texture &&o ) = default;
Texture::~Texture()                        = default;

SDL_Surface *Texture::getSurface() const { return m_surface; }
