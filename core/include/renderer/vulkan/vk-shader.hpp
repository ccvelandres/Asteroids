#pragma once

#include <memory>
#include <utils/file.hpp>
#include <vulkan/vulkan.hpp>

#include "vk-device.hpp"

class VulkanShaderModule
{
private:
    struct Internal;
    std::unique_ptr<Internal> m_internal;

protected:
public:
    VulkanShaderModule( VulkanDevice &device, const std::string &filename );
    ~VulkanShaderModule();

    const vk::ShaderModule &getShaderModule() const;

    operator vk::ShaderModule() { return getShaderModule(); }
};