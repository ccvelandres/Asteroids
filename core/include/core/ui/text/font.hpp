#pragma once

/**
 * @file core/ui/text.font.hpp
 * @author Cedric Velandres (ccvelandres@gmail.com)
 *
 * @addtogroup UI
 * @{
 */

#include <core/graphics/renderer/renderer.hpp>

namespace core::ui
{

    /**
     * @brief The Font enables to play/hear audio
     *
     */
    class Font
    {
    private:
        AssetID m_textureID;
    protected:
        /** Protected Constructors (use entity to add components) */
        Font();
    public:

        ~Font();
        Font(Font &o)             = delete;
        Font &operator=(Font &o)  = delete;
        Font(Font &&o)            = default;
        Font &operator=(Font &&o) = default;
    };

} // namespace core::graphics

/** @} endgroup Components */
