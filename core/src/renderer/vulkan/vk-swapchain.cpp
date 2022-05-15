#include <renderer/vulkan/vk-swapchain.hpp>
#include <renderer/vulkan/vk-utils.hpp>
#include <SDL2/SDL_vulkan.h>

#include <utils/logging.hpp>

#include <limits>
#include <algorithm>

vk::SurfaceFormatKHR VulkanSwapchain::chooseSurfaceFormat(std::vector<vk::SurfaceFormatKHR> &surfaceFormats)
{
    L_TAG("VulkanSwapchain::chooseSurfaceFormat");

    constexpr vk::Format preferredFormat =
        vk::Format::eB8G8R8A8Srgb;
    constexpr vk::ColorSpaceKHR preferredColorSpace =
        vk::ColorSpaceKHR::eSrgbNonlinear;

    if (surfaceFormats.size() == 0)
    {
        L_THROW_RUNTIME("No compatible surface formats found.");
    }

    // Attempt to look for preferred format and colorspace
    for (const auto &format : surfaceFormats)
    {
        if (format.format == preferredFormat &&
            format.colorSpace == preferredColorSpace)
        {
            return format;
        }
    }

    vk::SurfaceFormatKHR defaultFormat = surfaceFormats[0];

    //  if there's only one surface with undefined, we manually choose one
    if (surfaceFormats.size() == 1 &&
        defaultFormat.format == vk::Format::eUndefined)
    {
        defaultFormat.format = vk::Format::eB8G8R8A8Srgb;
        defaultFormat.colorSpace = vk::ColorSpaceKHR::eSrgbNonlinear;
        return defaultFormat;
    }

    // If we could not find suitable format, choose first one
    L_DEBUG("Could not find optimal SurfaceFormatKHR, choosing first one...");
    return defaultFormat;
}

vk::PresentModeKHR VulkanSwapchain::choosePresentMode(std::vector<vk::PresentModeKHR> &presentModes)
{
    L_TAG("VulkanSwapchain::choosePresentMode");

    // Setup order of preferred modes to check
    constexpr vk::PresentModeKHR preferredMode = vk::PresentModeKHR::eMailbox;

    // Check if preferred is available
    for (const auto &mode : presentModes)
    {
        if (mode == preferredMode)
        {
            return mode;
        }
    }

    // We use eFifo as default since it's guaranteed to be available
    return vk::PresentModeKHR::eFifo;
}

vk::Extent2D VulkanSwapchain::getSwapExtent(SDL_Window *window,
                                            const vk::SurfaceCapabilitiesKHR &surfaceCapabilities)
{
    L_TAG("VulkanSwapchain::VulkanSwapchain");

    if (surfaceCapabilities.currentExtent.width !=
        std::numeric_limits<uint32_t>::max())
    {
        return surfaceCapabilities.currentExtent;
    }
    else
    {
        int width, height;
        SDL_Vulkan_GetDrawableSize(window, &width, &height);

        vk::Extent2D extent;
        extent.width = std::clamp(static_cast<uint32_t>(width),
                                  surfaceCapabilities.minImageExtent.width,
                                  surfaceCapabilities.maxImageExtent.width);
        extent.height = std::clamp(static_cast<uint32_t>(height),
                                   surfaceCapabilities.minImageExtent.height,
                                   surfaceCapabilities.maxImageExtent.height);

        return extent;
        ;
    }
}

VulkanSwapchain::VulkanSwapchain(SDL_Window *window,
                                 VulkanInstance &instance,
                                 VulkanPhysicalDevice &physicalDevice,
                                 VulkanSurface &surface,
                                 VulkanDevice &device)
{
    L_TAG("VulkanSwapchain::VulkanSwapchain");

    vk::SurfaceFormatKHR format;
    vk::PresentModeKHR presentMode;
    vk::Extent2D extent;

    vk::SurfaceCapabilitiesKHR surfaceCapabilities =
        physicalDevice.getPhysicalDevice().getSurfaceCapabilitiesKHR(surface);

    L_DEBUG("minImageCount: {}", surfaceCapabilities.minImageCount);
    L_DEBUG("maxImageCount: {}", surfaceCapabilities.maxImageCount);
    
    uint32_t minImageCount = surfaceCapabilities.minImageCount + 1;
    if (surfaceCapabilities.maxImageCount == 0)
        minImageCount = 4; // Request 4 images if there's no limit
    else if (minImageCount > surfaceCapabilities.maxImageCount)
        minImageCount = surfaceCapabilities.maxImageCount;
    L_DEBUG("Requesting {} images for the swapchain", minImageCount);

    std::vector<vk::SurfaceFormatKHR> surfaceFormats =
        physicalDevice.getPhysicalDevice().getSurfaceFormatsKHR(surface);
    std::vector<vk::PresentModeKHR> presentModes =
        physicalDevice.getPhysicalDevice().getSurfacePresentModesKHR(surface);

    format = chooseSurfaceFormat(surfaceFormats);
    L_DEBUG("vk::Format: {}", vk::to_string(format.format));
    L_DEBUG("vk::ColorSpaceKHR: {}", vk::to_string(format.colorSpace));

    presentMode = choosePresentMode(presentModes);
    L_DEBUG("vk::PresentModeKHR: {}", vk::to_string(presentMode));

    extent = getSwapExtent(window, surfaceCapabilities);
    L_DEBUG("SwapExtent: {},{}", extent.width, extent.height);

    vk::SwapchainCreateInfoKHR swapchainCreateInfo;
    swapchainCreateInfo.setSurface(surface)
        .setMinImageCount(surfaceCapabilities.minImageCount + 1)
        .setImageFormat(format.format)
        .setImageColorSpace(format.colorSpace)
        .setImageExtent(extent)
        .setImageArrayLayers(1)
        .setImageUsage(vk::ImageUsageFlagBits::eColorAttachment)
        .setImageSharingMode(vk::SharingMode::eConcurrent)
        .setPreTransform(surfaceCapabilities.currentTransform)
        .setCompositeAlpha(vk::CompositeAlphaFlagBitsKHR::eOpaque)
        .setPresentMode(presentMode)
        .setClipped(VK_TRUE);

    uint32_t queueFamilyIndices[] =
        {
            device.getGraphicsQueueIndex(),
            device.getPresentQueueIndex()};

    if (queueFamilyIndices[0] != queueFamilyIndices[1])
    {
        swapchainCreateInfo.setImageSharingMode(vk::SharingMode::eConcurrent)
            .setQueueFamilyIndexCount(2)
            .setPQueueFamilyIndices(queueFamilyIndices);
    }

    m_swapchain = device.getDevice().createSwapchainKHRUnique(swapchainCreateInfo);
    L_DEBUG("VulkanSwapchain successfully created");
}

VulkanSwapchain::~VulkanSwapchain()
{
}

vk::SwapchainKHR &VulkanSwapchain::getSwapchain()
{
    return *m_swapchain;
}