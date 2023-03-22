#include <graphics/window.hpp>
#include <utils/logging.hpp>

namespace core::graphics
{
    struct Window::Internal
    {
        std::unique_ptr<Renderer> m_renderer;

        Internal(std::unique_ptr<Renderer> renderer) : m_renderer(std::move(renderer)) {}
    };

    Window::Window(std::unique_ptr<Renderer> renderer)
        : m_internal(std::make_unique<Internal>(std::move(renderer)))
    {
        L_TAG("Window::Window");
        L_TRACE("Window created");
    }

    Window::~Window()
    {
        L_TAG("Window::~Window");
        L_TRACE("Window destroyed");
    }

    Renderer &Window::renderer() const { return *(this->m_internal->m_renderer.get()); }
} // namespace core::graphics