#pragma once

/**
 * @file core/assets/sprite.hpp
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
    enum class SpriteSheetFormat
    {
        AsepriteArray,
        AsepriteHash
    };

    /**
     * @brief Helper class for loading sprite sheets from Aseprite
     * @todo: support trimmed size, as of now, currently we only support fixed size frames
     *
     */
    class SpriteSheet
    {
    public:
        struct Tag
        {
            std::size_t              from;
            std::size_t              to;
            bool                     direction;
            std::vector<std::size_t> frames;
        };

        struct Layer
        {
            int                      opacity;
            std::string              blendMode;
            std::vector<std::size_t> frames;
        };

        struct Frame
        {
            std::string name;
            bool        rotated;
            bool        trimmed;
            struct
            {
                std::size_t x; // x position of sprite
                std::size_t y; // y position of sprite
                std::size_t w; // width of sprite
                std::size_t h; // height of sprite
            } frame, spriteSourceSize;
            struct
            {
                std::size_t w; // width of sprite
                std::size_t h; // height of sprite
            } sourceSize;
            std::size_t duration; // duration of sprite in animation
        };

        struct SpriteData
        {
            std::string        sheetName;
            std::string        texture;
            std::string        textureFormat;
            std::vector<Frame> frames;
            std::unordered_map<std::string, Layer> layers;
            std::unordered_map<std::string, Tag>   tags;
        };
    private:
        SpriteData m_sheetData;
    protected:
    public:
        SpriteSheet();
        SpriteSheet(const AssetName &name);
        SpriteSheet(SDL_Surface *const surface, const SpriteData &spriteData);
        ~SpriteSheet();

        SpriteSheet(SpriteSheet &o)             = delete;
        SpriteSheet &operator=(SpriteSheet &o)  = delete;
        SpriteSheet(SpriteSheet &&o)            = default;
        SpriteSheet &operator=(SpriteSheet &&o) = default;

        SDL_Surface *getSurface() const;
        const std::string& getTextureName() const;
        const SpriteData& getSpriteData() const;
    };
} // namespace core::assets

/** @} endgroup Assets */
