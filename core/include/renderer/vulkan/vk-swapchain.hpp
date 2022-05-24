#pragma once

#include <SDL2/SDL.h>
#include <vulkan/vulkan.hpp>

#include "vk-device.hpp"
#include "vk-instance.hpp"
#include "vk-physicalDevice.hpp"
#include "vk-surface.hpp"

#include <vector>

class VulkanSwapchain
{
private:
    vk::UniqueSwapchainKHR m_swapchain;
    std::vector<vk::Image> m_images;
    vk::SurfaceFormatKHR   m_format;
    vk::PresentModeKHR     m_presentMode;
    vk::Extent2D           m_extent;

    static vk::SurfaceFormatKHR
    chooseSurfaceFormat( std::vector<vk::SurfaceFormatKHR> &surfaceFormats );
    static vk::PresentModeKHR choosePresentMode( std::vector<vk::PresentModeKHR> &presentModes );
    static vk::Extent2D       getSwapExtent( SDL_Window                       *window,
                                             const vk::SurfaceCapabilitiesKHR &surfaceCapabilities );

protected:
public:
    VulkanSwapchain( SDL_Window           *window,
                     VulkanInstance       &instance,
                     VulkanPhysicalDevice &physicalDevice,
                     VulkanSurface        &surface,
                     VulkanDevice         &device );
    ~VulkanSwapchain();

    vk::SwapchainKHR       &getSwapchain();
    std::vector<vk::Image> &getImages();
    vk::SurfaceFormatKHR   &getFormat();
    vk::PresentModeKHR     &getPresentMode();
    vk::Extent2D           &getExtent();

    operator vk::SwapchainKHR() { return getSwapchain(); }
};