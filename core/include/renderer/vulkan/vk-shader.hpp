#pragma once

#include <memory>
#include <utils/file.hpp>
#include <vulkan/vulkan.hpp>

#include "vk-device.hpp"

class VulkanShaderModule
{
private:
    const vk::UniqueShaderModule m_shader;

protected:
public:
    VulkanShaderModule( VulkanDevice &device, const std::string &filename );
    ~VulkanShaderModule();

    const vk::ShaderModule &getShaderModule() const;

    operator vk::ShaderModule() { return getShaderModule(); }
};