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
    /** @brief Valid window renderer types */
    enum class WindowRenderer
    {
        UNKNOWN,
        OpenGL2,
        OpenGL3,
        Vulkan,
    };

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
        Window(WindowRenderer     renderer,
               const std::string &windowTitle,
               const int          windowWidth  = 1280,
               const int          windowHeight = 720);
        Window(std::unique_ptr<Internal> internal);
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

        /** @brief Returns reference to renderer */
        Renderer &renderer() const;
    };
} // namespace core::graphics

/** @} endgroup Graphics */
