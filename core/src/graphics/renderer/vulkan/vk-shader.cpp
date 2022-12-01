#include <core/graphics/renderer/vulkan/vk-config.hpp>
#include <core/graphics/renderer/vulkan/vk-shader.hpp>
#include <core/graphics/renderer/vulkan/vk-utils.hpp>

#include <assets/utils.hpp>
#include <utils/logging.hpp>

#include <exception>

vk::UniqueShaderModule createShaderModule(const vk::Device &device, const std::string &filename)
{
    L_TAG("createShaderModule");

    /** @todo exception handling, maybe define a proper exception and throw again */
    auto bytecode = core::assets::utils::loadBinaryFile(filename);

    vk::ShaderModuleCreateInfo shaderCreateInfo = {};
    shaderCreateInfo.setCodeSize(bytecode.size()).setPCode(reinterpret_cast<uint32_t *>(bytecode.data()));

    return device.createShaderModuleUnique(shaderCreateInfo);
}

struct VulkanShaderModule::Internal
{
    const std::string            filename;
    const vk::UniqueShaderModule shader;

    Internal(VulkanDevice &device, const std::string &filename)
        : filename(filename),
          shader(::createShaderModule(device, filename))
    {
        L_TAG("VulkanShaderModule::Internal");
        L_DEBUG("Shader loaded ({})", filename);
        L_TRACE("Internal resources initialized ({})", static_cast<void *>(this));
    }

    ~Internal()
    {
        L_TAG("VulkanShaderModule::~Internal");
        L_DEBUG("Shader unloaded ({})", filename);
        L_TRACE("Internal resources freed ({})", static_cast<void *>(this));
    }
};

VulkanShaderModule::VulkanShaderModule(VulkanDevice &device, const std::string &filename)
    : m_internal(std::make_unique<Internal>(device, filename))
{
}

VulkanShaderModule::VulkanShaderModule(VulkanShaderModule &&o)            = default;
VulkanShaderModule &VulkanShaderModule::operator=(VulkanShaderModule &&o) = default;
VulkanShaderModule::~VulkanShaderModule()                                 = default;

const vk::ShaderModule &VulkanShaderModule::getShaderModule() const { return *m_internal->shader; }