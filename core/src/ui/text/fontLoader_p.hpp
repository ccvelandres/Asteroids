#pragma once

/**
 * @file core/assets/fontLoader_p.hpp
 * @author Cedric Velandres (ccvelandres@gmail.com)
 * @brief Private header for the FontLoader class
 * @addtogroup Assets
 * @{
 */

#include <ui/text/fontLoader.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <SDL_surface.h>

#include <string>

namespace core::ui
{
    struct FontLoader::Internal
    {
        std::string        m_name;
        std::size_t        m_startIndex;
        std::size_t        m_endIndex;
        FT_Library         m_library;
        FT_Face            m_face;

        // Loaded data
        std::vector<Glyph> m_glyphs;
        SDL_Surface       *m_atlasSurface;
        std::size_t        m_pixelCount;
    };
} // namespace core::ui

/** @} endgroup Assets */
