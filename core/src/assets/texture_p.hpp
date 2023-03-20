#pragma once

/**
 * @file core/assets/texture_p.hpp
 * @author Cedric Velandres (ccvelandres@gmail.com)
 * @brief Private header for the texture class
 * @addtogroup Assets
 * @{
 */

#include <SDL.h>

#include <assets/texture.hpp>

namespace core::assets
{
    struct Texture::Internal
    {
        std::string  m_name;
        SDL_Surface *m_surface;
    };
} // namespace core::assets

/** @} endgroup Assets */
