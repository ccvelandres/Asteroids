#include <renderer/vulkan/vk-context.hpp>
#include <renderer/vulkan/vk-rendercontext.hpp>
#include <renderer/vulkan/vk-utils.hpp>

#include <utils/logging.hpp>

struct VulkanRenderContext::internal {
    // const VulkanSwapchain  m_swapchain;
    // const VulkanImageView  m_imageview;
    // const VulkanRenderPass m_renderpass;

    internal() {}
};

VulkanRenderContext::VulkanRenderContext( SDL_Window           *window,
                                          VulkanInstance       &instance,
                                          VulkanPhysicalDevice &physicalDevice,
                                          VulkanDevice         &device,
                                          VulkanSurface        &surface,
                                          VulkanCommandPool    &commandPool )
    : m_swapchain( window, instance, physicalDevice, surface, device ),
      m_imageview( device, m_swapchain ),
      m_renderpass( physicalDevice, device, m_swapchain )
{
    L_TAG( "VulkanRenderContext::VulkanRenderContext" );
}

VulkanRenderContext::~VulkanRenderContext() {}
