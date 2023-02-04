#pragma once

/**
 * @file core/ui/text/fontLoader_p.hpp
 * @author Cedric Velandres (ccvelandres@gmail.com)
 *
 * @addtogroup UI
 * @{
 */

#include <ft2build.h>
#include FT_FREETYPE_H

#include <glm/glm.hpp>

#include "SDL_surface.h"

namespace core::ui
{
    enum class FontStyle
    {
        NORMAL,
        BOLD,
        ITALIC,
        UNDERLINE,
        STRIKETHROUGH
    };

    enum class FontHinting
    {
        NORMAL,
        LIGHT,
        MONO,
        NONE
    };

    enum class FontDirection
    {
        LEFT_TO_RIGHT,
        RIGHT_TO_LEFT,
        TOP_TO_BOTTOM,
        BOTTON_TO_TOP
    };

    struct Glyph
    {
        char charCode;
        glm::ivec2 size;
        glm::ivec2 bearing;
        unsigned int advance;
    };

    class FontLoader
    {
    private:
        FT_Library m_library;
        FT_Face    m_face;
    protected:
    public:

        FontLoader();
        ~FontLoader();
        FontLoader(const FontLoader &)            = delete;
        FontLoader &operator=(const FontLoader &) = delete;
        FontLoader(FontLoader &&)                 = delete;
        FontLoader &operator=(FontLoader &&)      = delete;

        int loadFont(const std::string &font);

        SDL_Surface* generateGlyphAtlas(std::size_t startIndex, std::size_t endIndex);

        FontLoader &setPixelSize(std::size_t width, std::size_t height);
        FontLoader &setCharSize(std::size_t charWidth,
                                std::size_t charHeight,
                                std::size_t horizontalRes,
                                std::size_t verticalRes);
        FontLoader &setFontKerning(bool enable);
        FontLoader &setFontOutline(const int &outline);
        FontLoader &setFontStyle();

        std::size_t getNumGlyphs() const noexcept;
    };
} // namespace core::ui

/** @} endgroup UI */
