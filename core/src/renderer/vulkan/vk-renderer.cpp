#include <renderer/vulkan/vk-config.hpp>
#include <renderer/vulkan/vk-context.hpp>
#include <renderer/vulkan/vk-renderer.hpp>
#include <renderer/vulkan/vk-utils.hpp>

#include <utils/logging.hpp>

#include <exception>

struct VulkanRenderer::Internal
{
    const SDL_Window *const window;
    const VulkanContext     context;

    Internal( SDL_Window *window ) : window( window ), context( window )
    {
        L_TAG( "VulkanRenderer::Internal" );
        L_DEBUG( "Renderer created" );
    }

    ~Internal()
    {
        L_TAG( "VulkanRenderer::~Internal" );
        L_DEBUG( "Internal data cleaned up" );
    }
};

VulkanRenderer::VulkanRenderer( SDL_Window *window )
    : m_internal( std::make_unique<Internal>( window ) )
{
}

VulkanRenderer::~VulkanRenderer() {}