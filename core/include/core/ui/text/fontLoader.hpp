#pragma once

/**
 * @file core/ui/text/fontLoader_p.hpp
 * @author Cedric Velandres (ccvelandres@gmail.com)
 *
 * @addtogroup UI
 * @{
 */

#include <core/assets/font.hpp>
#include <glm/glm.hpp>

#include <vector>

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

    class FontLoader
    {
    public:
        struct Glyph
        {
            std::size_t                charCode;
            glm::ivec2                 position;
            glm::ivec2                 size;
            glm::ivec2                 bearing;
            std::size_t                advance;
            std::size_t                pitch;
            std::vector<unsigned char> bitmap;
        };

        struct Internal;
    private:
        std::unique_ptr<Internal> m_internal;
    protected:
    public:

        FontLoader();
        ~FontLoader();
        FontLoader(const FontLoader &)            = delete;
        FontLoader &operator=(const FontLoader &) = delete;
        FontLoader(FontLoader &&)                 = delete;
        FontLoader &operator=(FontLoader &&)      = delete;

        FontLoader &openFont(const std::string &font,
                             std::size_t        startIndex = 33,
                             std::size_t        endIndex   = 127);
        FontLoader &setPixelSize(std::size_t width, std::size_t height);
        FontLoader &setCharSize(std::size_t charWidth,
                                std::size_t charHeight,
                                std::size_t horizontalRes,
                                std::size_t verticalRes);
        FontLoader &setFontKerning(bool enable);
        FontLoader &setFontOutline(const int &outline);
        FontLoader &setFontStyle();

        core::assets::Font generateFont();

        std::size_t getNumGlyphs() const noexcept;
    };
} // namespace core::ui

/** @} endgroup UI */
