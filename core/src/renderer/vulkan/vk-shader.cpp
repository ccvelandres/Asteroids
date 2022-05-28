#include <renderer/vulkan/vk-config.hpp>
#include <renderer/vulkan/vk-shader.hpp>
#include <renderer/vulkan/vk-utils.hpp>

#include <utils/file.hpp>
#include <utils/logging.hpp>

#include <exception>

vk::UniqueShaderModule createShaderModule( const vk::Device  &device,
                                           const std::string &filename )
{
    L_TAG( "createShaderModule" );

    /** @todo exception handling, maybe define a proper exception and throw again */
    auto bytecode = utils::readBinaryFile( filename );

    vk::ShaderModuleCreateInfo shaderCreateInfo = {};
    shaderCreateInfo.setCodeSize( bytecode.size() )
        .setPCode( reinterpret_cast<uint32_t *>( bytecode.data() ) );

    return device.createShaderModuleUnique( shaderCreateInfo );
}

VulkanShaderModule::VulkanShaderModule( VulkanDevice      &device,
                                        const std::string &filename )
    : m_shader( ::createShaderModule( device, filename ) )
{
    L_TAG( "VulkanShaderModule::VulkanShaderModule" );
    L_DEBUG( "ShaderModule created ({})", filename );
}

VulkanShaderModule::~VulkanShaderModule() {}

const vk::ShaderModule &VulkanShaderModule::getShaderModule() const
{
    return *m_shader;
}