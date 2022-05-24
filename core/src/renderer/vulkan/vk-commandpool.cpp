#include <renderer/vulkan/vk-commandpool.hpp>
#include <renderer/vulkan/vk-renderpass.hpp>
#include <renderer/vulkan/vk-utils.hpp>

#include <utils/logging.hpp>

vk::UniqueCommandPool VulkanCommandPool::createCommandPool( vk::Device &device, uint32_t queueIndex )
{
    vk::CommandPoolCreateInfo commandPoolCreateInfo =
        vk::CommandPoolCreateInfo( vk::CommandPoolCreateFlagBits::eResetCommandBuffer, queueIndex );

    return device.createCommandPoolUnique( commandPoolCreateInfo );
}

VulkanCommandPool::VulkanCommandPool( VulkanDevice &device ) : m_device( device )
{
    L_TAG( "VulkanCommandPool::VulkanCommandPool" );

    m_commandPool = createCommandPool( device.getDevice(), device.getGraphicsQueueIndex() );

    L_DEBUG("VulkanCommandPool created");
}

VulkanCommandPool::~VulkanCommandPool() {}

vk::UniqueCommandBuffer VulkanCommandPool::createCommandBuffer()
{
    vk::CommandBufferAllocateInfo commandBufferAllocateInfo =
        vk::CommandBufferAllocateInfo( *m_commandPool, vk::CommandBufferLevel::ePrimary, 1 );

    vk::UniqueCommandBuffer buffer( std::move(
        m_device.getDevice().allocateCommandBuffersUnique( commandBufferAllocateInfo ) [0] ) );

    vk::CommandBufferBeginInfo beginInfo =
        vk::CommandBufferBeginInfo( vk::CommandBufferUsageFlagBits::eOneTimeSubmit, nullptr );

    // Request to begin the buffer
    buffer->begin( beginInfo );
    return buffer;
}

void VulkanCommandPool::endCommandBuffer( vk::CommandBuffer &commandBuffer )
{
    /** Stop the command buffer */
    commandBuffer.end();
    /** Submit command buffer to graphics queue */
    vk::SubmitInfo submitInfo( 0, nullptr, nullptr, 1, &commandBuffer, 0, nullptr );
    m_device.getGraphicsQueue().submit( 1, &submitInfo, vk::Fence() );
    /** Wait will the graphics queue is idle */
    m_device.getGraphicsQueue().waitIdle();
}