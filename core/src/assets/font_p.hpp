#pragma once

/**
 * @file core/assets/font_p.hpp
 * @author Cedric Velandres (ccvelandres@gmail.com)
 * @brief Private header for the font class
 * @addtogroup Assets
 * @{
 */

#include <SDL.h>

#include <assets/font.hpp>

#include <map>

namespace core::assets
{
    struct Font::Internal
    {
        std::string        m_name;
        std::size_t        m_glyphOffset;
        std::vector<Glyph> m_glyphs;

        SDL_Surface *m_atlas;
    };
} // namespace core::assets

/** @} endgroup Assets */
