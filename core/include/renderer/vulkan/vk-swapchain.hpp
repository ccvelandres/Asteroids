#pragma once

#include <SDL2/SDL.h>
#include <vulkan/vulkan.hpp>

#include "vk-instance.hpp"
#include "vk-phyiscalDevice.hpp"
#include "vk-surface.hpp"
#include "vk-device.hpp"

class VulkanSwapchain
{
private:
    vk::UniqueSwapchainKHR m_swapchain;

    static vk::SurfaceFormatKHR chooseSurfaceFormat(std::vector<vk::SurfaceFormatKHR> &surfaceFormats);
    static vk::PresentModeKHR choosePresentMode(std::vector<vk::PresentModeKHR> &presentModes);
    static vk::Extent2D getSwapExtent(SDL_Window *window,
                                      const vk::SurfaceCapabilitiesKHR &surfaceCapabilities);

protected:
public:
    VulkanSwapchain(SDL_Window *window,
                    VulkanInstance &instance,
                    VulkanPhysicalDevice &physicalDevice,
                    VulkanSurface &surface,
                    VulkanDevice &device);
    ~VulkanSwapchain();

    vk::SwapchainKHR &getSwapchain();
    operator vk::SwapchainKHR() { return getSwapchain(); }
};