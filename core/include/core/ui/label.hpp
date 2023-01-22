#pragma once

/**
 * @file core/ui/label.hpp
 * @author Cedric Velandres (ccvelandres@gmail.com)
 *
 * @addtogroup UI
 * @{
 */

#include <glm/glm.hpp>

namespace core::ui
{

    /**
     * @brief The Label enables to play/hear audio
     *
     */
    class Label
    {
    private:
        glm::vec2 m_rectPos;
        glm::vec2 m_rectSize;
    protected:
        /** Protected Constructors (use entity to add components) */
        Label();
        Label(const glm::vec2& rectPos, const glm::vec2& rectSize);
    public:

        ~Label();
        Label(Label &o)             = delete;
        Label &operator=(Label &o)  = delete;
        Label(Label &&o)            = default;
        Label &operator=(Label &&o) = default;
    };

} // namespace core::graphics

/** @} endgroup Components */
