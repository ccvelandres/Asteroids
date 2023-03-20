#include <ui/text/fontLoader.hpp>
#include <ui/text/fontLoader_p.hpp>
#include <assets/font_p.hpp>
#include <utils/logging.hpp>

#include <SDL_image.h>

#include <algorithm>
#include <vector>

namespace core::ui
{
    static void setGrayscalePalette(SDL_Surface *surface)
    {
        L_TAG("setGrayscalePalette");
        constexpr std::size_t paletteSize = 256;
        SDL_Color             colors[256];

        for (std::size_t i = 0; i < paletteSize; ++i)
        {
            colors[i].r = colors[i].a = colors[i].g = colors[i].b = i;
        }

        SDL_SetPaletteColors(surface->format->palette, colors, 0, paletteSize);
    }

    static int createAtlasSurface(const std::unique_ptr<core::ui::FontLoader::Internal> &fontLoader)
    {
        L_TAG("FontLoader::createAtlasSurface");
        std::size_t atlasWidth  = 1;
        std::size_t atlasHeight = 1;

        while (atlasHeight * atlasWidth < fontLoader->m_pixelCount)
        {
            if (atlasWidth == atlasHeight)
                atlasWidth <<= 1;
            else
                atlasHeight <<= 1;
        }

        L_TRACE("Allocating atlas surface ({}x{})", atlasWidth, atlasHeight);
        SDL_Surface *textureAtlas =
            SDL_CreateRGBSurfaceWithFormat(0, atlasWidth, atlasHeight, 8, SDL_PIXELFORMAT_INDEX8);
        if (nullptr == textureAtlas)
        {
            L_TRACE("Could not create surface for font atlas");
            return EXIT_FAILURE;
        }

        // Set surface palette
        setGrayscalePalette(textureAtlas);

        // Free atlas if non null
        if (nullptr != fontLoader->m_atlasSurface) SDL_FreeSurface(fontLoader->m_atlasSurface);
        fontLoader->m_atlasSurface = textureAtlas;
        return EXIT_SUCCESS;
    }

    static int buildGlyphs(const std::unique_ptr<core::ui::FontLoader::Internal> &fontLoader,
                           FT_Render_Mode                                         renderMode)
    {
        L_TAG("FontLoader::buildGlyphs");
        // TODO: support unicode characters
        L_ASSERT(fontLoader->m_face != nullptr, "No active face");

        auto &face       = fontLoader->m_face;
        auto  startIndex = fontLoader->m_startIndex;
        auto  endIndex   = fontLoader->m_endIndex;
        auto &glyphs     = fontLoader->m_glyphs;

        // Reserve items for glpyh vector
        glyphs = std::vector<core::ui::FontLoader::Glyph>(endIndex - startIndex);

        std::size_t destX      = 0;
        std::size_t destY      = 0;
        std::size_t pixelCount = 0;

        for (std::size_t i = startIndex; i < endIndex; ++i)
        {
            std::size_t                  glyphIndex = FT_Get_Char_Index(face, i);
            core::ui::FontLoader::Glyph &glyphEntry = glyphs.at(i - startIndex);

            if (FT_Load_Glyph(face, glyphIndex, FT_LOAD_DEFAULT)
                || FT_Render_Glyph(face->glyph, renderMode))
            {
                L_TRACE("Could not render glyph: {}", glyphIndex);
                return EXIT_FAILURE;
            }

            auto &glyph  = face->glyph;
            auto &bitmap = face->glyph->bitmap;

            glyphEntry.charCode = i;
            glyphEntry.size     = glm::ivec2(bitmap.width, bitmap.rows);
            glyphEntry.bearing  = glm::ivec2(glyph->bitmap_left, glyph->bitmap_top);
            glyphEntry.advance  = glyph->advance.x >> 6;
            glyphEntry.pitch    = bitmap.pitch;
            glyphEntry.bitmap =
                std::vector<unsigned char>(bitmap.buffer,
                                           bitmap.buffer + (bitmap.rows * bitmap.pitch));

            pixelCount += bitmap.pitch * bitmap.rows;
        }
        L_TRACE("Built {} glyphs for total of {} pixels", glyphs.size(), pixelCount);

        fontLoader->m_pixelCount = pixelCount;
        return EXIT_SUCCESS;
    }

    static int packGlyphs(const std::unique_ptr<core::ui::FontLoader::Internal> &fontLoader)
    {
        L_TAG("FontLoader::packGlyphs");

        // Create surface
        L_ASSERT(createAtlasSurface(fontLoader) == EXIT_SUCCESS,
                 "Could not create surface for atlas");

        auto &glyphs       = fontLoader->m_glyphs;
        auto &atlasSurface = fontLoader->m_atlasSurface;

        std::size_t destX     = 0;
        std::size_t destY     = 0;
        std::size_t maxHeight = 0;

        L_TRACE("Packing {} glyphs to surface ({}x{})",
                glyphs.size(),
                atlasSurface->w,
                atlasSurface->h);
        if (SDL_MUSTLOCK(atlasSurface)) SDL_LockSurface(atlasSurface);

        unsigned char *pixels = static_cast<unsigned char *>(atlasSurface->pixels);
        for (auto &glyph : glyphs)
        {
            if (destX + glyph.size.x > atlasSurface->w)
            {
                destX = 0;
                destY += maxHeight;
                maxHeight = 0;
            }

            if (destY + glyph.size.y > atlasSurface->h)
            {
                // TODO: recreate bigger atlas if this happens
                L_TRACE("glyph {} exceeds atlasHeight", glyph.charCode);
                return EXIT_FAILURE;
            }

            for (std::size_t row = 0; row < glyph.size.y; ++row)
            {
                // This copy expects surface to be index8 and glyph bitmap to be single channel - greyscale
                std::size_t y = destY + row;
                memcpy(&pixels[((destY + row) * atlasSurface->w) + destX],
                       &glyph.bitmap[(row * glyph.pitch)],
                       glyph.size.x);
            }

            destX += glyph.size.x;
            maxHeight = std::max(maxHeight, static_cast<std::size_t>(glyph.size.y));
        }

        if (SDL_MUSTLOCK(atlasSurface)) SDL_UnlockSurface(atlasSurface);
        return EXIT_SUCCESS;
    }

    static int buildFontAsset(const std::unique_ptr<core::ui::FontLoader::Internal> &fontLoader,
                              const std::unique_ptr<core::assets::Font::Internal>   &fontInternal)
    {
        L_TAG("FontLoader::buildFontAsset");

        fontInternal->m_atlas       = fontLoader->m_atlasSurface;
        fontInternal->m_glyphOffset = fontLoader->m_startIndex;
        fontInternal->m_glyphs.reserve(fontLoader->m_glyphs.size());
        std::transform(fontLoader->m_glyphs.begin(),
                       fontLoader->m_glyphs.end(),
                       fontInternal->m_glyphs.begin(),
                       [](const auto &in) {
                           return core::assets::Font::Glyph{.charCode = in.charCode,
                                                            .position = in.position,
                                                            .size     = in.size,
                                                            .bearing  = in.bearing,
                                                            .advance  = in.advance};
                       });

        // Clear loader data
        fontLoader->m_atlasSurface = nullptr;
        fontLoader->m_glyphs.clear();
        fontLoader->m_pixelCount = 0;
        return EXIT_SUCCESS;
    }

    // Public API
    FontLoader::FontLoader() : m_internal(std::make_unique<Internal>())
    {
        L_TAG("FontLoader::FontLoader");
        FT_Error err;
        auto    &library = this->m_internal->m_library;

        err = FT_Init_FreeType(&library);
        if (err != 0)
        {
            L_THROW_RUNTIME("Could not initialize FT Library");
        }
        L_TRACE("Internal resources initialized ({})", static_cast<void *>(this));
    }
    FontLoader::~FontLoader()
    {
        L_TAG("FontLoader::FontLoader");
        if (nullptr != this->m_internal->m_atlasSurface)
            SDL_FreeSurface(this->m_internal->m_atlasSurface);
        if (nullptr != this->m_internal->m_face) FT_Done_Face(this->m_internal->m_face);
        if (nullptr != this->m_internal->m_library) FT_Done_FreeType(this->m_internal->m_library);
        L_TRACE("Internal resources freed ({})", static_cast<void *>(this));
    }

    FontLoader &FontLoader::openFont(const std::string &font,
                                     std::size_t        startIndex,
                                     std::size_t        endIndex)
    {
        L_TAG("FontLoader::openFont");
        L_ASSERT(endIndex > startIndex, "startIndex must be greater than endIndex");

        FT_Error err;
        FT_Face  face;
        auto    &library = this->m_internal->m_library;

        err = FT_New_Face(library, font.c_str(), 0, &face);
        if (err != 0)
        {
            L_THROW_RUNTIME("Could not open font face: {}", err);
        }

        // Close active face
        if (nullptr != this->m_internal->m_face) FT_Done_Face(this->m_internal->m_face);
        if (nullptr != this->m_internal->m_atlasSurface)
            SDL_FreeSurface(this->m_internal->m_atlasSurface);

        this->m_internal->m_name         = font;
        this->m_internal->m_face         = face;
        this->m_internal->m_startIndex   = startIndex;
        this->m_internal->m_endIndex     = endIndex;
        this->m_internal->m_atlasSurface = nullptr;
        this->m_internal->m_glyphs.clear();

        // Set default pixel size to 32x32
        this->setPixelSize(0, 24);
        return *this;
    }

    FontLoader &FontLoader::setPixelSize(std::size_t width, std::size_t height)
    {
        L_TAG("FontLoader::setPixelSize");
        L_ASSERT(this->m_internal->m_face != nullptr, "No active face");

        auto &face = this->m_internal->m_face;

        FT_Error err = FT_Set_Pixel_Sizes(face, width, height);
        if (err != 0)
        {
            L_THROW_RUNTIME("Could not set pixel size");
        }

        L_TRACE("Set pixel size ({},{}) for {}", width, height, this->m_internal->m_name);
        return *this;
    }

    FontLoader &FontLoader::setCharSize(std::size_t charWidth,
                                        std::size_t charHeight,
                                        std::size_t horizontalRes,
                                        std::size_t verticalRes)
    {
        L_TAG("FontLoader::setCharSize");
        L_ASSERT(this->m_internal->m_face != nullptr, "No active face");

        auto    &face = this->m_internal->m_face;
        FT_Error err  = FT_Set_Char_Size(face, charWidth, charHeight, horizontalRes, verticalRes);
        if (err != 0)
        {
            L_THROW_RUNTIME("Could not set char size");
        }

        return *this;
    }

    core::assets::Font FontLoader::generateFont()
    {
        // TODO: support unicode characters
        L_TAG("FontLoader::generateFont");

        // Build glyphs to pack
        if (buildGlyphs(this->m_internal, FT_RENDER_MODE_SDF))
            L_THROW_RUNTIME("Could not build font glyphs");

        // Pack glyphs
        if (packGlyphs(this->m_internal)) L_THROW_RUNTIME("Could not pack font glyphs");

        // Build font asset
        std::unique_ptr<core::assets::Font::Internal> fontInternal =
            std::make_unique<core::assets::Font::Internal>();

        if (buildFontAsset(this->m_internal, fontInternal))
            L_THROW_RUNTIME("Could not build font asset");

        IMG_SavePNG(fontInternal->m_atlas, "font_atlas.png");

        return core::assets::Font(std::move(fontInternal));
    }

    std::size_t FontLoader::getNumGlyphs() const noexcept
    {
        return this->m_internal->m_face->num_glyphs;
    }

} // namespace core::ui