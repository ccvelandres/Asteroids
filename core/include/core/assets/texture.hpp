#pragma once

/**
 * @file core/assets/texture.hpp
 * @author Cedric Velandres (ccvelandres@gmail.com)
 *
 * @addtogroup Assets
 * @{
 */

#include <string>
#include <SDL2/SDL_surface.h>

#include "asset-inventory.hpp"

namespace core::assets
{
    /**
     * @brief Helper class for loading texture files
     * 
     */
    class Texture
    {
    public:
        struct TextureData
        {
            SDL_Surface *surface;
        };
    private:
        TextureData m_textureData;
    protected:
    public:
        Texture();
        Texture(const AssetName &name);
        Texture(SDL_Surface *const surface);
        ~Texture();

        Texture(Texture &o)             = delete;
        Texture &operator=(Texture &o)  = delete;
        Texture(Texture &&o)            = default;
        Texture &operator=(Texture &&o) = default;

        SDL_Surface *getSurface() const;
    };
} // namespace core::assets

/** @} endgroup Assets */
