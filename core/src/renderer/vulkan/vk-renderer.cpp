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
        L_TRACE( "Internal resources initialized ({})", static_cast<void *>( this ) );
    }

    ~Internal()
    {
        L_TAG( "VulkanRenderer::~Internal" );
        L_TRACE( "Internal resources freed ({})", static_cast<void *>( this ) );
    }
};

VulkanRenderer::VulkanRenderer( SDL_Window *window ) : m_internal( std::make_unique<Internal>( window ) ) {}

VulkanRenderer::~VulkanRenderer() = default;

void VulkanRenderer::init() {}

void VulkanRenderer::update( const time_ms delta ) {}

void VulkanRenderer::clean() {}

void VulkanRenderer::refresh() {}

void VulkanRenderer::renderBegin() {}

void VulkanRenderer::render() {}

void VulkanRenderer::renderEnd() {}
