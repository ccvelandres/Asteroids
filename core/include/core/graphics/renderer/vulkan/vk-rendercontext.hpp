#pragma once

#include <memory>
#include <SDL.h>
#include <vulkan/vulkan.hpp>

#include "vk-commandpool.hpp"
#include "vk-device.hpp"
#include "vk-image.hpp"
#include "vk-instance.hpp"
#include "vk-physicalDevice.hpp"
#include "vk-renderpass.hpp"
#include "vk-surface.hpp"
#include "vk-swapchain.hpp"

class VulkanRenderContext
{
private:
    struct Internal;
    std::unique_ptr<Internal> m_internal;

protected:
public:
    VulkanRenderContext(VulkanRenderContext &&o);
    VulkanRenderContext &operator=(VulkanRenderContext &&o);
    VulkanRenderContext(SDL_Window *const           window,
                        const VulkanInstance       &instance,
                        const VulkanPhysicalDevice &physicalDevice,
                        const VulkanDevice         &device,
                        const VulkanSurface        &surface,
                        const VulkanCommandPool    &commandPool,
                        const vk::SwapchainKHR     &oldSwapchain = vk::SwapchainKHR());
    ~VulkanRenderContext();

    VulkanRenderContext recreate(SDL_Window *const           window,
                                 const VulkanInstance       &instance,
                                 const VulkanPhysicalDevice &physicalDevice,
                                 const VulkanDevice         &device,
                                 const VulkanSurface        &surface,
                                 const VulkanCommandPool    &commandPool);

    bool renderBegin(const VulkanDevice &device) const;
    bool renderEnd(const VulkanDevice &device) const;
};