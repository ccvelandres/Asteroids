#pragma once

/**
 * @file core/graphics/window.hpp.hpp
 * @author Cedric Velandres (ccvelandres@gmail.com)
 *
 * @addtogroup Graphics
 * @{
 */

#include "renderer.hpp"
#include <core/time.hpp>
#include <memory>

namespace core::graphics
{

    /**
     * @brief Window object
     */
    class Window
    {
    public:
        struct Internal;
    private:
        std::unique_ptr<Internal> m_internal;
    protected:
    public:
        Window(std::unique_ptr<Renderer> renderer);
        ~Window();
        Window(Window &o)             = delete;
        Window(Window &&o)            = delete;
        Window &operator=(Window &o)  = delete;
        Window &operator=(Window &&o) = delete;

        // void init();
        // void preUpdate();
        // void fixedUpdate(const time_ms &delta);
        // void update(const time_ms &delta);
        // void postUpdate();
        // void refresh();

        Renderer &renderer() const;
    };
} // namespace core::graphics

/** @} endgroup Graphics */
