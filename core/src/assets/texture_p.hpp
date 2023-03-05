#pragma once

/**
 * @file core/assets/texture_p.hpp
 * @author Cedric Velandres (ccvelandres@gmail.com)
 * @brief Private header for the texture class
 * @addtogroup Assets
 * @{
 */

#include <assets/texture.hpp>

#include <SDL2/SDL.h>

namespace core::assets
{
    struct Texture::Internal
    {
        std::string  m_name;
        SDL_Surface *m_surface;

        Internal(const AssetName &name);
        ~Internal();
    };
} // namespace core::assets

/** @} endgroup Audio */
