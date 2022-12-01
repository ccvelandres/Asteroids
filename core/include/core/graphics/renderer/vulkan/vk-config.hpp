#pragma once

#include <vector>
#include <vulkan/vulkan.hpp>

struct VulkanConfig
{
    static const vk::ApplicationInfo      *getApplicationInfo();
    static const std::vector<const char *> getValidationLayers();
    static const std::vector<const char *> getInstanceExtensions();
};