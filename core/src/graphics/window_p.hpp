#pragma once

/**
 * @file core/graphics/window_p.hpp
 * @author Cedric Velandres (ccvelandres@gmail.com)
 * @brief Private header for the Window class
 * @addtogroup Graphics
 * @{
 */

#include <graphics/window.hpp>

#include <SDL.h>

namespace core::graphics
{
    struct Window::Internal
    {
        SDL_Window               *m_window;
        std::string               m_windowTitle;
        std::size_t               m_windowWidth;
        std::size_t               m_windowHeight;
        std::unique_ptr<Renderer> m_renderer;
    };
} // namespace core::graphics

/** @} endgroup Graphics */
