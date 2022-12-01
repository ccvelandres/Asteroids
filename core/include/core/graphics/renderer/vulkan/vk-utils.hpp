#pragma once

#include <vulkan/vulkan.hpp>

struct VulkanUtils
{
    static bool checkSwapchainSupport(vk::PhysicalDevice &device);
    static bool checkValidationLayerSupport(const char                             *validationLayers,
                                            const std::vector<vk::LayerProperties> &availableLayers);
};