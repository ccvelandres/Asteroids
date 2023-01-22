#pragma once

/**
 * @file core/ui/element.hpp
 * @author Cedric Velandres (ccvelandres@gmail.com)
 *
 * @addtogroup UI
 * @{
 */

namespace core::ui
{

    /**
     * @brief The Element enables to play/hear audio
     *
     */
    class Element
    {
    private:

    protected:
        /** Protected Constructors (use entity to add components) */
        Element();
    public:

        ~Element();
        Element(Element &o)             = delete;
        Element &operator=(Element &o)  = delete;
        Element(Element &&o)            = default;
        Element &operator=(Element &&o) = default;
    };

} // namespace core::graphics

/** @} endgroup Components */
