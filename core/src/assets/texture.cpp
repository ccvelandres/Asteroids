#include <core/assets/texture.hpp>
#include <core/utils/logging.hpp>

#include <assets/texture_p.hpp>

#include <SDL_image.h>

namespace core::assets
{
    static SDL_Surface *load_file(const AssetName &name)
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

        return surface;
    }

    Texture::Internal::Internal(const AssetName& name)
    {
        L_TAG("Texture(&name)");

        const AssetPaths &assetPaths =
            AssetInventory::getInstance().lookupAssets(AssetType::Texture, name);

        L_ASSERT(assetPaths.size() == 1, "Found multiple paths for {}", name);
        auto &assetPath = assetPaths.at(0);

        this->m_surface = load_file(assetPath);
    }

    Texture::Internal::~Internal()
    {
        if (this->m_surface) SDL_FreeSurface(this->m_surface);
    }

    Texture::Texture() = default;
    Texture::~Texture()
    {
        L_TAG("~Texture");
        L_TRACE("Internal resources freed ({})", static_cast<void *>(this));
    };

    Texture::Texture(const AssetName &name)
     : m_internal(std::make_unique<Internal>(name))
    {
        L_TAG("Texture(&name)");
        L_TRACE("Internal resources initialized ({})", static_cast<void *>(this));
    }

    const std::string &Texture::name() const noexcept { return this->m_internal->m_name; }
    const Texture::Internal &Texture::getInternal() const noexcept { return *(this->m_internal); }
} // namespace core::assets
