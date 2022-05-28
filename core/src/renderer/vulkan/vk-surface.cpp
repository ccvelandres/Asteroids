#include <renderer/vulkan/vk-surface.hpp>
#include <renderer/vulkan/vk-utils.hpp>
#include <SDL2/SDL_vulkan.h>
#include <utils/logging.hpp>

vk::UniqueSurfaceKHR createSurface( SDL_Window *window, const vk::Instance &instance )
{

    L_TAG( "createSurface" );

    VkSurfaceKHR surface;
    if ( SDL_Vulkan_CreateSurface( window, instance, &surface ) != SDL_TRUE )
    {
        L_THROW_RUNTIME( "Could not create Vulkan Surface" );
    }

    return vk::UniqueSurfaceKHR( surface, instance );
}

VulkanSurface::VulkanSurface( SDL_Window *window, const VulkanInstance &instance )
    : m_surface( createSurface( window, instance.getInstance() ) )
{
}

VulkanSurface::~VulkanSurface() {}

const vk::SurfaceKHR &VulkanSurface::getSurface() const { return *m_surface; }