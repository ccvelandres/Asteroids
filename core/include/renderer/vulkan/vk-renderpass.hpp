#pragma once

#include <SDL2/SDL.h>
#include <vulkan/vulkan.hpp>

#include "vk-device.hpp"
#include "vk-physicalDevice.hpp"
#include "vk-swapchain.hpp"

class VulkanRenderPass
{
private:
    struct Internal;
    std::unique_ptr<Internal> m_internal;

protected:
public:
    VulkanRenderPass( VulkanRenderPass && );
    VulkanRenderPass &operator=( VulkanRenderPass &&other );
    VulkanRenderPass( const VulkanPhysicalDevice &physicalDevice,
                      const VulkanDevice         &device,
                      const VulkanSwapchain      &swapchain );
    ~VulkanRenderPass();

    const vk::RenderPass &getRenderPass() const;
};