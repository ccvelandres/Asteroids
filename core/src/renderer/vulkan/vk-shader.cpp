#include <renderer/vulkan/vk-config.hpp>
#include <renderer/vulkan/vk-shader.hpp>
#include <renderer/vulkan/vk-utils.hpp>

#include <utils/file.hpp>
#include <utils/logging.hpp>

#include <exception>

VulkanShaderModule::VulkanShaderModule(VulkanDevice &device,
                                       const std::string &filename)
{
    L_TAG("VulkanShaderModule::VulkanShaderModule");

    /** @todo exception handling, maybe define a proper exception and throw again */
    auto bytecode = utils::readBinaryFile(filename);

    vk::ShaderModuleCreateInfo shaderCreateInfo = {};
    shaderCreateInfo.setCodeSize(bytecode.size())
        .setPCode(reinterpret_cast<uint32_t *>(bytecode.data()));

    m_shader = device.getDevice().createShaderModuleUnique(shaderCreateInfo);
}

VulkanShaderModule::~VulkanShaderModule()
{
}

vk::ShaderModule &VulkanShaderModule::getShaderModule()
{
    return *m_shader;
}