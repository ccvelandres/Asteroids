#include <renderer/vulkan/vk-config.hpp>
#include <renderer/vulkan/vk-context.hpp>
#include <renderer/vulkan/vk-utils.hpp>

#include <utils/logging.hpp>

struct VulkanContext::Internal
{
    const VulkanInstance       m_instance;
    const VulkanPhysicalDevice m_physicalDevice;
    const VulkanSurface        m_surface;
    const VulkanDevice         m_device;
    const VulkanCommandPool    m_commandPool;
    const VulkanRenderContext  m_renderContext;

    Internal( SDL_Window *window )
        : m_instance( window ),
          m_physicalDevice( m_instance ),
          m_surface( window, m_instance ),
          m_device( window, m_physicalDevice, m_surface ),
          m_commandPool( m_device ),
          m_renderContext( window,
                           m_instance,
                           m_physicalDevice,
                           m_device,
                           m_surface,
                           m_commandPool )
    {
        L_TAG( "VulkanContext::Internal" );
        L_TRACE( "Internal resources initialized ({})", static_cast<void*>(this) );
    }

    ~Internal()
    {
        L_TAG( "VulkanContext::~Internal" );
        L_TRACE( "Internal resources freed ({})", static_cast<void*>(this) );
    }
};

VulkanContext::VulkanContext( SDL_Window *window )
    : m_internal( std::make_unique<Internal>( window ) )
{
}

VulkanContext::~VulkanContext() {}
