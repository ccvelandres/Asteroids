#pragma once

/**
 * @file core/ui/text/character.hpp
 * @author Cedric Velandres (ccvelandres@gmail.com)
 *
 * @addtogroup UI
 * @{
 */

namespace core::ui
{

    /**
     * @brief The Character enables to play/hear audio
     *
     */
    class Character
    {
    private:

    protected:
        /** Protected Constructors (use entity to add components) */
        Character();
    public:

        ~Character();
        Character(Character &o)             = delete;
        Character &operator=(Character &o)  = delete;
        Character(Character &&o)            = default;
        Character &operator=(Character &&o) = default;
    };

} // namespace core::graphics

/** @} endgroup Components */
