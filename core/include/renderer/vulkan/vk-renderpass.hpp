#pragma once

#include <SDL2/SDL.h>
#include <vulkan/vulkan.hpp>

#include "vk-device.hpp"
#include "vk-physicalDevice.hpp"
#include "vk-swapchain.hpp"

class VulkanRenderPass
{
private:
    vk::UniqueRenderPass m_renderpass;
    
protected:
public:
    VulkanRenderPass(VulkanPhysicalDevice &physicalDevice,
                        VulkanDevice &device,
                        VulkanSwapchain &swapchain);
    ~VulkanRenderPass();
};