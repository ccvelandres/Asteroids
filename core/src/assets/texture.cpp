#include <assets/texture.hpp>

#include <utils/logging.hpp>

namespace assets
{
    Texture::Texture()  = default;
    Texture::~Texture() = default;

    Texture::Texture(const std::string &path)
    {
        L_TAG("Texture::Texture");
        SDL_Surface *source = IMG_Load(path.c_str());
        if (source == NULL) L_THROW_RUNTIME("Could not load texture file {}", path);
        SDL_Rect imageFrame = {0, 0, source->w, source->h};

#if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
        const uint32_t redMask   = 0xff000000;
        const uint32_t greenMask = 0x00ff0000;
        const uint32_t blueMask  = 0x0000ff00;
        const uint32_t alphaMask = 0x000000ff;
#else
        const uint32_t redMask   = 0x000000ff;
        const uint32_t greenMask = 0x0000ff00;
        const uint32_t blueMask  = 0x00ff0000;
        const uint32_t alphaMask = 0xff000000;
#endif

        SDL_Surface *surface =
            SDL_CreateRGBSurface(0, imageFrame.w, imageFrame.h, 32, redMask, greenMask, blueMask, alphaMask);
        if (surface == NULL) L_THROW_RUNTIME("Could not create RGB Surface {}", path);

        SDL_BlitSurface(source, &imageFrame, surface, &imageFrame);
        SDL_FreeSurface(source);

        L_DEBUG("Texture loaded from {}", path);

        m_surface = surface;
    }

    SDL_Surface *Texture::getSurface() const { return m_surface; }
} // namespace assets
