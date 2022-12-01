#include <core/graphics/renderer/vulkan/vk-utils.hpp>

bool VulkanUtils::checkSwapchainSupport(vk::PhysicalDevice &device)
{
    // Check if device supports swapchain
    bool        swapchainSupport = false;
    std::string swapchainName    = VK_KHR_SWAPCHAIN_EXTENSION_NAME;

    for (const auto &extension : device.enumerateDeviceExtensionProperties())
    {
        if (strcmp(extension.extensionName, swapchainName.c_str()) == 0)
        {
            swapchainSupport = true;
            break;
        }
    }

    return swapchainSupport;
}

bool VulkanUtils::checkValidationLayerSupport(const char                             *validationLayer,
                                              const std::vector<vk::LayerProperties> &availableLayers)
{
    for (const auto &layer : availableLayers)
    {
        if (strcmp(validationLayer, layer.layerName) == 0)
        {
            return true;
        }
    }
    return false;
}