#include <renderer/opengl/gl-texture.hpp>
#include <utils/logging.hpp>

#include "SDL_opengl.h"

#include <vector>

struct OpenGLTexture::Internal
{

    Internal( const Texture &texture )
    {
        L_TAG( "OpenGLTexture::Internal" );
        L_TRACE( "Internal resources initialized ({})", static_cast<void *>( this ) );
    }

    ~Internal()
    {
        L_TAG( "OpenGLTexture::~Internal" );
        L_TRACE( "Internal resources freed ({})", static_cast<void *>( this ) );
    }
};

OpenGLTexture::OpenGLTexture( const Texture &texture ) 
    : m_internal( std::make_unique<Internal>( texture ) )
    {}

OpenGLTexture::OpenGLTexture( OpenGLTexture &&o ) = default;
OpenGLTexture &OpenGLTexture::operator=( OpenGLTexture &&o ) = default;
OpenGLTexture::~OpenGLTexture()                               = default;


