#include <renderer/vulkan/vk-imageview.hpp>
#include <renderer/vulkan/vk-utils.hpp>
#include <SDL2/SDL_vulkan.h>

#include <utils/logging.hpp>

#include <limits>
#include <algorithm>

VulkanImageView::VulkanImageView(SDL_Window *window,
                                 VulkanInstance &instance,
                                 VulkanPhysicalDevice &physicalDevice,
                                 VulkanSurface &surface,
                                 VulkanDevice &device,
                                 VulkanSwapchain &swapchain)
{
    L_TAG("VulkanImageView::VulkanImageView");

    std::vector<vk::Image> images = swapchain.getImages();

    m_imageViews.resize(images.size());

    
}

VulkanImageView::~VulkanImageView()
{
    
}
