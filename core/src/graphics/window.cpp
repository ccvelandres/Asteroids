#include <graphics/window.hpp>
#include <graphics/window_p.hpp>
#include <utils/logging.hpp>

#if defined(CONFIG_CORE_RENDERER_VULKAN)
#include <core/graphics/renderer/vulkan/vk-renderer.hpp>
#endif

#if defined(CONFIG_CORE_RENDERER_OPENGL)
#include <core/graphics/renderer/opengl/gl-renderer.hpp>
#endif

namespace core::graphics
{
    void createGLWindow(const std::unique_ptr<Window::Internal> &windowInternal)
    {
        L_TAG("Window::createGLWindow");

        auto &window       = windowInternal->m_window;
        auto &renderer     = windowInternal->m_renderer;
        auto &windowTitle  = windowInternal->m_windowTitle;
        auto &windowWidth  = windowInternal->m_windowWidth;
        auto &windowHeight = windowInternal->m_windowHeight;

        SDL_WindowFlags glFlags = OpenGLRenderer::getWindowFlags();

        window = SDL_CreateWindow(windowTitle.c_str(),
                                  SDL_WINDOWPOS_CENTERED,
                                  SDL_WINDOWPOS_CENTERED,
                                  windowWidth,
                                  windowHeight,
                                  SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | glFlags);
        if (nullptr == window)
        {
            L_THROW_RUNTIME("Could not create window");
        }

        renderer = std::make_unique<OpenGLRenderer>(
            OpenGLRenderer(window, windowTitle, windowWidth, windowHeight));
    }

    Window::Window(WindowRenderer     renderer,
                   const std::string &windowTitle,
                   const int          windowWidth,
                   const int          windowHeight)
        : m_internal(std::make_unique<Internal>())
    {
        L_TAG("Window::Window");
        this->m_internal->m_windowTitle  = windowTitle;
        this->m_internal->m_windowWidth  = windowWidth;
        this->m_internal->m_windowHeight = windowHeight;

        switch (renderer)
        {
        case WindowRenderer::OpenGL2:
        case WindowRenderer::OpenGL3:
#if defined(CONFIG_CORE_RENDERER_OPENGL)
            core::graphics::createGLWindow(this->m_internal);
#else
            L_UNIMPLEMENTED("OpenGLRenderer not enabled");
#endif
            break;
        case WindowRenderer::Vulkan:
#if defined(CONFIG_CORE_RENDERER_VULKAN)
            L_UNIMPLEMENTED("VulkanRenderer still wip");
#else
            L_UNIMPLEMENTED("VulkanRenderer not enabled");
#endif
            break;
        case WindowRenderer::UNKNOWN:

            break;
        default:
            L_UNIMPLEMENTED("Unknown WindowRenderer");
            break;
        }

        L_TRACE("Window created");
    }

    Window::Window(std::unique_ptr<Internal> internal) : m_internal(std::move(internal)) {}

    Window::~Window()
    {
        L_TAG("Window::~Window");
        L_TRACE("Window destroyed");
    }

    Renderer &Window::renderer() const { return *(this->m_internal->m_renderer.get()); }
} // namespace core::graphics