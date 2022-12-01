#pragma once

#include <memory>
#include <assets/utils.hpp>
#include <vulkan/vulkan.hpp>

#include "vk-device.hpp"

class VulkanShaderModule
{
private:
    struct Internal;
    std::unique_ptr<Internal> m_internal;

protected:
public:
    VulkanShaderModule(VulkanShaderModule &&o);
    VulkanShaderModule &operator=(VulkanShaderModule &&o);
    VulkanShaderModule(VulkanDevice &device, const std::string &filename);
    ~VulkanShaderModule();

    const vk::ShaderModule &getShaderModule() const;

    operator vk::ShaderModule() { return getShaderModule(); }
};