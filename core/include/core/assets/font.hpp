#pragma once

/**
 * @file core/assets/font.hpp
 * @author Cedric Velandres (ccvelandres@gmail.com)
 *
 * @addtogroup Assets
 * @{
 */

#include <memory>
#include <cstdint>
#include <vector>

#include <glm/glm.hpp>
#include "asset-inventory.hpp"

namespace core::assets
{
    /**
     * @brief Helper class for parsing mesh files
     *
     */
    class Font
    {
    public:
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
            std::size_t charCode;
            glm::ivec2  position;
            glm::ivec2  size;
            glm::ivec2  bearing;
            std::size_t advance;
        };

        struct Internal;
    private:
        std::unique_ptr<Internal> m_internal;
    public:
        Font(const AssetName &name);
        Font(std::unique_ptr<Internal> internal);
        Font();
        ~Font();

        Font(const Font &)            = delete;
        Font &operator=(const Font &) = delete;
        Font(Font &&o);
        Font &operator=(Font &&o);

        Font &setPixelSize(std::size_t width, std::size_t height);
        Font &setCharSize(std::size_t charWidth,
                          std::size_t charHeight,
                          std::size_t horizontalRes,
                          std::size_t verticalRes);
        Font &setFontKerning(bool enable);
        Font &setFontOutline(const int &outline);
        Font &setFontStyle();

        std::size_t getNumGlyphs() const noexcept;

        /** @brief Returns the texture name */
        const std::string &name() const noexcept;
    };
} // namespace core::assets

/** @} endgroup Assets */