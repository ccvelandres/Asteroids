#pragma once

/**
 * @file core/assets/texture.hpp
 * @author Cedric Velandres (ccvelandres@gmail.com)
 *
 * @addtogroup Assets
 * @{
 */

#include <string>
#include <SDL_surface.h>

#include "asset-inventory.hpp"

namespace core::assets
{
    /** Forward declaration for Model */
    class Model;

    /**
     * @brief Helper class for loading texture files
     *
     */
    class Texture
    {
    private:
        std::string  m_name;
        SDL_Surface *m_surface;
    protected:
    public:
        Texture();
        Texture(const AssetName &name);
        Texture(const std::string &name, SDL_Surface *const surface);
        ~Texture();

        Texture(Texture &o)             = delete;
        Texture &operator=(Texture &o)  = delete;
        Texture(Texture &&o)            = default;
        Texture &operator=(Texture &&o) = default;

        /** @brief Returns the texture name */
        const std::string &name() const noexcept;
        /** @brief Returns the sdl surface */
        SDL_Surface *getSurface() const noexcept;

        friend Model;
    };
} // namespace core::assets

/** @} endgroup Assets */
