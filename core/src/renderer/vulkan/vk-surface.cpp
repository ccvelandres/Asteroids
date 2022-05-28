#include <renderer/vulkan/vk-surface.hpp>
#include <renderer/vulkan/vk-utils.hpp>
#include <SDL2/SDL_vulkan.h>
#include <utils/logging.hpp>

vk::UniqueSurfaceKHR createSurface( SDL_Window         *window,
                                    const vk::Instance &instance )
{

    L_TAG( "createSurface" );

    VkSurfaceKHR surface;
    if ( SDL_Vulkan_CreateSurface( window, instance, &surface ) != SDL_TRUE )
    {
        L_THROW_RUNTIME( "Could not create Vulkan Surface" );
    }

    return vk::UniqueSurfaceKHR( surface, instance );
}

struct VulkanSurface::Internal
{
    const vk::UniqueSurfaceKHR surface;

    Internal( SDL_Window *window, const vk::Instance &instance )
        : surface( ::createSurface( window, instance ) )
    {
        L_TAG( "VulkanSurface::Internal" );
        L_DEBUG( "SurfaceKHR successfully created" );
    }
};

VulkanSurface::VulkanSurface( SDL_Window *window, const VulkanInstance &instance )
    : m_internal( std::make_unique<Internal>( window, instance.getInstance() ) )
{
}

VulkanSurface::~VulkanSurface() {}

const vk::SurfaceKHR &VulkanSurface::getSurface() const
{
    return *m_internal->surface;
}