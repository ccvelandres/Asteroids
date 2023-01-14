#include <core/assets/texture.hpp>
#include <core/utils/logging.hpp>

#include <SDL_image.h>

namespace core::assets
{
    static void load_file(const AssetName &name, Texture::TextureData &textureData)
    {
        L_TAG("Texture(file)");

        SDL_Surface *source = IMG_Load(name.c_str());
        if (source == NULL) L_THROW_RUNTIME("Could not load texture file {}", name);
        SDL_Rect imageFrame = {0, 0, source->w, source->h};

#if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
        constexpr uint32_t redMask   = 0xff000000;
        constexpr uint32_t greenMask = 0x00ff0000;
        constexpr uint32_t blueMask  = 0x0000ff00;
        constexpr uint32_t alphaMask = 0x000000ff;
#else
        constexpr uint32_t redMask   = 0x000000ff;
        constexpr uint32_t greenMask = 0x0000ff00;
        constexpr uint32_t blueMask  = 0x00ff0000;
        constexpr uint32_t alphaMask = 0xff000000;
#endif

        SDL_Surface *surface =
            SDL_CreateRGBSurface(0, imageFrame.w, imageFrame.h, 32, redMask, greenMask, blueMask, alphaMask);
        if (surface == NULL) L_THROW_RUNTIME("Could not create RGB Surface {}", name);

        SDL_BlitSurface(source, &imageFrame, surface, &imageFrame);
        SDL_FreeSurface(source);

        L_DEBUG("Texture loaded from {}", name);

        textureData.surface = surface;
    }

    Texture::Texture() = default;
    Texture::~Texture()
    {
        L_TAG("~Texture");
        if (m_textureData.surface) SDL_FreeSurface(m_textureData.surface);
        L_TRACE("Internal resources freed ({})", static_cast<void *>(this));
    };

    Texture::Texture(const AssetName &name)
    {
        L_TAG("Texture(&name)");

        /** unlike with mesh that we can store defaults to some array,
         * it's better that we just have default texture files then load
         * that normally
         */
        load_file(name, this->m_textureData);

        L_TRACE("Internal resources initialized ({})", static_cast<void *>(this));
    }

    Texture::Texture(SDL_Surface *const surface) : m_textureData{.surface = surface}
    {
        L_TAG("Texture(&surface)");
        L_TRACE("Internal resources initialized ({})", static_cast<void *>(this));
    }

    SDL_Surface *Texture::getSurface() const { return m_textureData.surface; }
} // namespace core::assets
