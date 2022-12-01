#include <core/graphics/renderer/vulkan/vk-config.hpp>

#include <utils/logging.hpp>

vk::ApplicationInfo applicationInfo{"ApplicationName",
                                    VK_MAKE_VERSION(1, 1, 0),
                                    "NoEngine",
                                    VK_MAKE_VERSION(1, 1, 0),
                                    VK_API_VERSION_1_2};

const std::vector<const char *> validationLayers = {"VK_LAYER_KHRONOS_validation"};

const std::vector<const char *> instanceExtensions = {VK_EXT_DEBUG_UTILS_EXTENSION_NAME};

const vk::ApplicationInfo *VulkanConfig::getApplicationInfo() { return &applicationInfo; }

const std::vector<const char *> VulkanConfig::getValidationLayers() { return validationLayers; }

const std::vector<const char *> VulkanConfig::getInstanceExtensions() { return instanceExtensions; }