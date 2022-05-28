#pragma once

#include <SDL2/SDL.h>
#include <vulkan/vulkan.hpp>

#include "vk-device.hpp"
#include "vk-physicalDevice.hpp"
#include "vk-swapchain.hpp"

class VulkanRenderPass
{
private:
    const vk::UniqueRenderPass m_renderpass;

protected:
public:
    VulkanRenderPass( const VulkanPhysicalDevice &physicalDevice,
                      const VulkanDevice         &device,
                      const VulkanSwapchain      &swapchain );
    ~VulkanRenderPass();
};