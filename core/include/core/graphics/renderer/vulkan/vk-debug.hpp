#pragma once

#include <memory>
#include <SDL.h>
#include <vulkan/vulkan.hpp>

class VulkanDebugUtils
{
private:
    struct Internal;
    std::unique_ptr<Internal> m_internal;

protected:
public:
    VulkanDebugUtils(VulkanDebugUtils &&o);
    VulkanDebugUtils &operator=(VulkanDebugUtils &&o);
    VulkanDebugUtils(const vk::Instance &instance);
    ~VulkanDebugUtils();
};