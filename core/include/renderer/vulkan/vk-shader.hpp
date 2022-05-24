#pragma once

#include <memory>
#include <utils/file.hpp>
#include <vulkan/vulkan.hpp>

#include "vk-device.hpp"

class VulkanShaderModule
{
private:
    vk::UniqueShaderModule m_shader;

protected:
public:
    VulkanShaderModule( VulkanDevice &device, const std::string &filename );
    ~VulkanShaderModule();

    vk::ShaderModule &getShaderModule();

    operator vk::ShaderModule() { return getShaderModule(); }
};