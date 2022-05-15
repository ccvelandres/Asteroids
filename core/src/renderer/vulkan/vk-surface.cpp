#include <renderer/vulkan/vk-surface.hpp>
#include <renderer/vulkan/vk-utils.hpp>
#include <SDL2/SDL_vulkan.h>
#include <utils/logging.hpp>

VulkanSurface::VulkanSurface(SDL_Window *window,
                             const vk::Instance &instance)
{
    L_TAG("VulkanSurface::VulkanSurface");

    VkSurfaceKHR surface;
    if (SDL_Vulkan_CreateSurface(window, instance, &surface) != SDL_TRUE)
    {
        L_THROW_RUNTIME("Could not create Vulkan Surface");
    }

    m_surface = vk::UniqueSurfaceKHR(surface, instance);
    L_DEBUG("VulkanSurface successfully created");
}

VulkanSurface::~VulkanSurface()
{
}

vk::SurfaceKHR &VulkanSurface::getSurface()
{
    return *m_surface;
}