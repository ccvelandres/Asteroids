#pragma once

#include <SDL.h>
#include <vulkan/vulkan.hpp>

#include "vk-device.hpp"
#include "vk-imageview.hpp"
#include "vk-instance.hpp"
#include "vk-physicalDevice.hpp"
#include "vk-surface.hpp"

#include <vector>

class VulkanSwapchain
{
private:
    struct Internal;
    std::unique_ptr<Internal> m_internal;

protected:
public:
    VulkanSwapchain(VulkanSwapchain &&o);
    VulkanSwapchain &operator=(VulkanSwapchain &&o);
    VulkanSwapchain(SDL_Window                 *window,
                    const VulkanInstance       &instance,
                    const VulkanPhysicalDevice &physicalDevice,
                    const VulkanSurface        &surface,
                    const VulkanDevice         &device,
                    const vk::SwapchainKHR     &oldSwapchain);
    ~VulkanSwapchain();

    const vk::SwapchainKHR             &getSwapchain() const;
    const std::vector<vk::Image>       &getImages() const;
    const std::vector<VulkanImageView> &getImageViews() const;
    const uint32_t                      getImageCount() const;
    const vk::SurfaceFormatKHR          getFormat() const;
    const vk::PresentModeKHR            getPresentMode() const;
    const vk::Extent2D                  getExtent() const;

    operator vk::SwapchainKHR() { return getSwapchain(); }
};