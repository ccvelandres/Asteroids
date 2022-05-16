#pragma once

#include <vulkan/vulkan.hpp>

#include "vk-instance.hpp"
#include "vk-phyiscalDevice.hpp"
#include "vk-surface.hpp"
#include "vk-device.hpp"
#include "vk-swapchain.hpp"
#include "vk-imageview.hpp"

class VulkanContext
{
private:
    VulkanInstance m_instance;
    VulkanPhysicalDevice m_physicalDevice;
    VulkanSurface m_surface;
    VulkanDevice m_device;
    VulkanSwapchain m_swapchain;
    VulkanImageView m_imageview;
protected:
public:
    VulkanContext(SDL_Window *window);
    ~VulkanContext();
};