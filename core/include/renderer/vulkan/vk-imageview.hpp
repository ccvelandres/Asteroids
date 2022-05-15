#pragma once

#include <SDL2/SDL.h>
#include <vulkan/vulkan.hpp>

#include "vk-instance.hpp"
#include "vk-phyiscalDevice.hpp"
#include "vk-surface.hpp"
#include "vk-device.hpp"
#include "vk-swapchain.hpp"

#include <vector>

class VulkanImageView
{
private:
    std::vector<vk::ImageView> m_imageViews;

protected:
public:
    VulkanImageView(SDL_Window *window,
                    VulkanInstance &instance,
                    VulkanPhysicalDevice &physicalDevice,
                    VulkanSurface &surface,
                    VulkanDevice &device,
                    VulkanSwapchain &swapchain);
                    
    ~VulkanImageView();

};