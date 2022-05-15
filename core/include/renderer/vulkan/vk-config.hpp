#pragma once

#include <vulkan/vulkan.hpp>
#include <vector> 

struct VulkanConfig {
    static const vk::ApplicationInfo *getApplicationInfo();
    static const std::vector<const char*> getValidationLayers();
    static const std::vector<const char*> getInstanceExtensions();
};