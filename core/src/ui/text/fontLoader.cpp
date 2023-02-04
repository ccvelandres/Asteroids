#include <core/utils/logging.hpp>

#include <core/ui/text/fontLoader.hpp>

#include <SDL_surface.h>

#include <vector>

namespace core::ui
{
    FontLoader::FontLoader()
    {
        L_TAG("FontLoader::FontLoader");

        FT_Error err = FT_Init_FreeType(&this->m_library);
        if (err != 0)
        {
            L_THROW_RUNTIME("Could not initialize FT Library");
        }
    }

    FontLoader::~FontLoader() {}

    int FontLoader::loadFont(const std::string &font)
    {
        L_TAG("FontLoader::loadFont");
        FT_Error err = FT_New_Face(this->m_library, font.c_str(), 0, &this->m_face);
        if (err != 0)
        {
            L_ERROR("Could not open font face: {}", err);
            return EXIT_FAILURE;
        }
    }

    SDL_Surface* FontLoader::generateGlyphAtlas(std::size_t startIndex, std::size_t endIndex)
    {
        L_TAG("FontLoader::generateGlyphAtlas");

        std::vector<Glyph> m_vecGlyph;
        m_vecGlyph.reserve(endIndex - startIndex);

        for (std::size_t i = startIndex; i < endIndex; ++i)
        {
            std::size_t glyphIndex = FT_Get_Char_Index(this->m_face, i);
            if (FT_Load_Glyph(this->m_face, glyphIndex, FT_LOAD_RENDER | FT_LOAD_MONOCHROME))
            {
                L_ERROR("Could not load glyph: {}", glyphIndex);
                continue;
            }

            auto &bitmap = this->m_face->glyph->bitmap;
            L_INFO("pixel_mode: {}", bitmap.pixel_mode);
            SDL_Surface *surface =
                SDL_CreateRGBSurfaceFrom(bitmap.buffer, bitmap.width, bitmap.rows, 1, bitmap.pitch, 0, 0, 0, 0x1);
            if(surface == NULL)
            {
                L_ERROR("Could not create surface");
            }

            return surface;
        }
    }

    FontLoader &FontLoader::setPixelSize(std::size_t width, std::size_t height)
    {
        L_TAG("FontLoader::setPixelSize");
        FT_Error err = FT_Set_Pixel_Sizes(this->m_face, width, height);
        if (err != 0)
        {
            L_THROW_RUNTIME("Could not set pixel size");
        }

        return *this;
    }

    FontLoader &FontLoader::setCharSize(std::size_t charWidth,
                                        std::size_t charHeight,
                                        std::size_t horizontalRes,
                                        std::size_t verticalRes)
    {
        L_TAG("FontLoader::setCharSize");
        FT_Error err = FT_Set_Char_Size(this->m_face, charWidth, charHeight, horizontalRes, verticalRes);
        if (err != 0)
        {
            L_THROW_RUNTIME("Could not set char size");
        }

        return *this;
    }

    std::size_t FontLoader::getNumGlyphs() const noexcept { return this->m_face->num_glyphs; }

} // namespace core::ui