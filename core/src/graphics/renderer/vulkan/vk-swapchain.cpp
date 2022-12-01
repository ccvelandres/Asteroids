#include <core/graphics/renderer/vulkan/vk-swapchain.hpp>
#include <core/graphics/renderer/vulkan/vk-utils.hpp>
#include <SDL2/SDL_vulkan.h>

#include <utils/logging.hpp>

#include <algorithm>
#include <limits>

vk::SurfaceFormatKHR chooseSurfaceFormat(const VulkanPhysicalDevice &physicalDevice, const VulkanSurface &surface)
{
    L_TAG("chooseSurfaceFormat");

    std::vector<vk::SurfaceFormatKHR> surfaceFormats =
        physicalDevice.getPhysicalDevice().getSurfaceFormatsKHR(surface.getSurface());

    constexpr vk::Format        preferredFormat     = vk::Format::eB8G8R8A8Srgb;
    constexpr vk::ColorSpaceKHR preferredColorSpace = vk::ColorSpaceKHR::eSrgbNonlinear;

    if (surfaceFormats.size() == 0)
    {
        L_THROW_RUNTIME("No compatible surface formats found.");
    }

    // Attempt to look for preferred format and colorspace
    for (const auto &format : surfaceFormats)
    {
        if (format.format == preferredFormat && format.colorSpace == preferredColorSpace)
        {
            return format;
        }
    }

    vk::SurfaceFormatKHR defaultFormat = surfaceFormats[0];

    //  if there's only one surface with undefined, we manually choose one
    if (surfaceFormats.size() == 1 && defaultFormat.format == vk::Format::eUndefined)
    {
        defaultFormat.format     = vk::Format::eB8G8R8A8Srgb;
        defaultFormat.colorSpace = vk::ColorSpaceKHR::eSrgbNonlinear;
        return defaultFormat;
    }

    // If we could not find suitable format, choose first one
    L_DEBUG("Could not find optimal SurfaceFormatKHR, choosing first one...");
    return defaultFormat;
}

vk::PresentModeKHR choosePresentMode(const VulkanPhysicalDevice &physicalDevice, const VulkanSurface &surface)
{
    L_TAG("choosePresentMode");

    std::vector<vk::PresentModeKHR> presentModes =
        physicalDevice.getPhysicalDevice().getSurfacePresentModesKHR(surface.getSurface());

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

vk::Extent2D getSwapExtent(SDL_Window *window, const VulkanPhysicalDevice &physicalDevice, const VulkanSurface &surface)
{
    L_TAG("getSwapExtent");

    vk::SurfaceCapabilitiesKHR surfaceCapabilities =
        physicalDevice.getPhysicalDevice().getSurfaceCapabilitiesKHR(surface.getSurface());

    if (surfaceCapabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
    {
        return surfaceCapabilities.currentExtent;
    }
    else
    {
        int width, height;
        SDL_Vulkan_GetDrawableSize(window, &width, &height);

        vk::Extent2D extent;
        extent.width  = std::clamp(static_cast<uint32_t>(width),
                                  surfaceCapabilities.minImageExtent.width,
                                  surfaceCapabilities.maxImageExtent.width);
        extent.height = std::clamp(static_cast<uint32_t>(height),
                                   surfaceCapabilities.minImageExtent.height,
                                   surfaceCapabilities.maxImageExtent.height);

        return extent;
    }
}

vk::UniqueSwapchainKHR createSwapchain(const VulkanPhysicalDevice &physicalDevice,
                                       const VulkanSurface        &surface,
                                       const VulkanDevice         &device,
                                       const vk::SurfaceFormatKHR &format,
                                       const vk::PresentModeKHR   &presentMode,
                                       const vk::Extent2D         &extent,
                                       const vk::SwapchainKHR     &oldSwapchain)
{
    L_TAG("createSwapchain");
    L_DEBUG("Using ImageFormat {}", vk::to_string(format.format));
    L_DEBUG("Using ImageColorSpace {}", vk::to_string(format.colorSpace));
    L_DEBUG("Using PresentModeKHR {}", vk::to_string(presentMode));
    L_DEBUG("Using Extents {},{}", extent.width, extent.height);

    /** Get surface capabilities of surface */
    vk::SurfaceCapabilitiesKHR surfaceCapabilities =
        physicalDevice.getPhysicalDevice().getSurfaceCapabilitiesKHR(surface.getSurface());

    /** Set the image count we request for the swapchain */
    uint32_t minImageCount = surfaceCapabilities.minImageCount + 1;
    if (surfaceCapabilities.maxImageCount == 0)
        minImageCount = 4; // Request 4 images if there's no limit
    else if (minImageCount > surfaceCapabilities.maxImageCount)
        minImageCount = surfaceCapabilities.maxImageCount + 1;
    L_DEBUG("Requesting {} images for the swapchain", minImageCount);

    vk::SwapchainCreateInfoKHR swapchainCreateInfo(vk::SwapchainCreateFlagsKHR(),
                                                   surface.getSurface(),
                                                   minImageCount,
                                                   format.format,
                                                   format.colorSpace,
                                                   extent,
                                                   1,
                                                   vk::ImageUsageFlagBits::eColorAttachment,
                                                   vk::SharingMode::eExclusive,
                                                   0,
                                                   nullptr,
                                                   surfaceCapabilities.currentTransform,
                                                   vk::CompositeAlphaFlagBitsKHR::eOpaque,
                                                   presentMode,
                                                   VK_TRUE,
                                                   oldSwapchain);

    uint32_t queueFamilyIndices[] = {device.getGraphicsQueueIndex(), device.getPresentQueueIndex()};

    if (queueFamilyIndices[0] != queueFamilyIndices[1])
    {
        swapchainCreateInfo.setImageSharingMode(vk::SharingMode::eConcurrent)
            .setQueueFamilyIndexCount(2)
            .setPQueueFamilyIndices(queueFamilyIndices);
    }

    return device.getDevice().createSwapchainKHRUnique(swapchainCreateInfo);
}

std::vector<VulkanImageView> createImageViews(const VulkanDevice           &device,
                                              const std::vector<vk::Image> &images,
                                              const vk::SwapchainKHR       &swapChain,
                                              const vk::SurfaceFormatKHR   &format)
{
    L_TAG("createImageViews");

    std::vector<VulkanImageView> imageViews;

    // For each of the images in the swap chain, we need to create a new 'image view'.
    L_TRACE("Creating imageViews for each swapchain images");
    for (const vk::Image &image : images)
    {
        VulkanImageView imageView(device, image, format.format, vk::ImageAspectFlagBits::eColor, 1);

        imageViews.push_back(std::move(imageView));
    }

    return imageViews;
}

struct VulkanSwapchain::Internal
{
    const vk::SurfaceFormatKHR         format;
    const vk::PresentModeKHR           presentMode;
    const vk::Extent2D                 extent;
    const vk::UniqueSwapchainKHR       swapchain;
    const std::vector<vk::Image>       images;
    const std::vector<VulkanImageView> imageViews;

    Internal(SDL_Window                 *window,
             const VulkanInstance       &instance,
             const VulkanPhysicalDevice &physicalDevice,
             const VulkanSurface        &surface,
             const VulkanDevice         &device,
             const vk::SwapchainKHR     &oldSwapchain)
        : format(::chooseSurfaceFormat(physicalDevice, surface)),
          presentMode(::choosePresentMode(physicalDevice, surface)),
          extent(::getSwapExtent(window, physicalDevice, surface)),
          swapchain(::createSwapchain(physicalDevice, surface, device, format, presentMode, extent, oldSwapchain)),
          images(device.getDevice().getSwapchainImagesKHR(*swapchain)),
          imageViews(::createImageViews(device, images, *swapchain, format))
    {
        L_TAG("VulkanSwapchain::Internal");
        L_TRACE("Internal resources initialized ({})", static_cast<void *>(this));
    }

    ~Internal()
    {
        L_TAG("VulkanSwapchain::~Internal");
        L_TRACE("Internal resources freed ({})", static_cast<void *>(this));
    }
};

VulkanSwapchain::VulkanSwapchain(SDL_Window                 *window,
                                 const VulkanInstance       &instance,
                                 const VulkanPhysicalDevice &physicalDevice,
                                 const VulkanSurface        &surface,
                                 const VulkanDevice         &device,
                                 const vk::SwapchainKHR     &oldSwapchain)
    : m_internal(std::make_unique<Internal>(window, instance, physicalDevice, surface, device, oldSwapchain))
{
    L_TAG("VulkanSwapchain::VulkanSwapchain");
}

VulkanSwapchain::VulkanSwapchain(VulkanSwapchain &&o)            = default;
VulkanSwapchain &VulkanSwapchain::operator=(VulkanSwapchain &&o) = default;
VulkanSwapchain::~VulkanSwapchain()                              = default;

const vk::SwapchainKHR &VulkanSwapchain::getSwapchain() const { return *m_internal->swapchain; }

const std::vector<vk::Image> &VulkanSwapchain::getImages() const { return m_internal->images; }

const std::vector<VulkanImageView> &VulkanSwapchain::getImageViews() const { return m_internal->imageViews; }
const uint32_t                      VulkanSwapchain::getImageCount() const { return m_internal->images.size(); }

const vk::SurfaceFormatKHR VulkanSwapchain::getFormat() const { return m_internal->format; }

const vk::PresentModeKHR VulkanSwapchain::getPresentMode() const { return m_internal->presentMode; }

const vk::Extent2D VulkanSwapchain::getExtent() const { return m_internal->extent; }