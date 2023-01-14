#include <core/graphics/renderer/vulkan/vk-surface.hpp>
#include <core/graphics/renderer/vulkan/vk-utils.hpp>
#include <SDL_vulkan.h>
#include <utils/logging.hpp>

vk::UniqueSurfaceKHR createSurface(SDL_Window *window, const vk::Instance &instance)
{

    L_TAG("createSurface");

    VkSurfaceKHR surface;
    if (SDL_Vulkan_CreateSurface(window, instance, &surface) != SDL_TRUE)
    {
        L_THROW_RUNTIME("Could not create Vulkan Surface");
    }

    return vk::UniqueSurfaceKHR(surface, instance);
}

struct VulkanSurface::Internal
{
    const vk::UniqueSurfaceKHR surface;

    Internal(SDL_Window *window, const vk::Instance &instance) : surface(::createSurface(window, instance))
    {
        L_TAG("VulkanSurface::Internal");
        L_TRACE("Internal resources initialized ({})", static_cast<void *>(this));
    }

    ~Internal()
    {
        L_TAG("VulkanSurface::~Internal");
        L_TRACE("Internal resources freed ({})", static_cast<void *>(this));
    }
};

VulkanSurface::VulkanSurface(SDL_Window *window, const VulkanInstance &instance)
    : m_internal(std::make_unique<Internal>(window, instance.getInstance()))
{
}

VulkanSurface::VulkanSurface(VulkanSurface &&o)            = default;
VulkanSurface &VulkanSurface::operator=(VulkanSurface &&o) = default;
VulkanSurface::~VulkanSurface()                            = default;

const vk::SurfaceKHR &VulkanSurface::getSurface() const { return *m_internal->surface; }