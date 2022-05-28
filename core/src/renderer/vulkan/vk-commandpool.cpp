#include <renderer/vulkan/vk-commandpool.hpp>
#include <renderer/vulkan/vk-renderpass.hpp>
#include <renderer/vulkan/vk-utils.hpp>

#include <utils/logging.hpp>

vk::UniqueCommandPool createCommandPool( const VulkanDevice &device )
{
    L_TAG( "createCommandPool" );

    /** Create a command pool for the device */
    vk::CommandPoolCreateInfo commandPoolCreateInfo = vk::CommandPoolCreateInfo(
        vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
        device.getGraphicsQueueIndex() );

    return device.getDevice().createCommandPoolUnique( commandPoolCreateInfo );
}

struct VulkanCommandPool::Internal
{
    const vk::UniqueCommandPool commandPool;

    Internal( const VulkanDevice &device )
        : commandPool( ::createCommandPool( device ) )
    {
        L_TAG( "VulkanCommandPool::Internal" );
        L_DEBUG( "CommandPool successfully created" );
    }
};

VulkanCommandPool::VulkanCommandPool( const VulkanDevice &device )
    : m_internal( std::make_unique<Internal>( device ) )
{
}

VulkanCommandPool::~VulkanCommandPool() {}

vk::UniqueCommandBuffer VulkanCommandPool::createCommandBuffer(
    const VulkanDevice &device ) const
{
    L_TAG( "VulkanCommandPool::createCommandBuffer" );

    vk::CommandBufferAllocateInfo commandBufferAllocateInfo =
        vk::CommandBufferAllocateInfo( *m_internal->commandPool,
                                       vk::CommandBufferLevel::ePrimary,
                                       1 );

    vk::UniqueCommandBuffer buffer(
        std::move( device.getDevice().allocateCommandBuffersUnique(
            commandBufferAllocateInfo ) [0] ) );

    vk::CommandBufferBeginInfo beginInfo = vk::CommandBufferBeginInfo(
        vk::CommandBufferUsageFlagBits::eOneTimeSubmit,
        nullptr );

    /** Request to begin the buffer */
    buffer->begin( beginInfo );
    L_TRACE( "({}) created and started", (void *)&buffer.get() );
    return buffer;
}

void VulkanCommandPool::endCommandBuffer( vk::CommandBuffer  &buffer,
                                          const VulkanDevice &device ) const
{
    L_TAG( "VulkanCommandPool::endCommandBuffer" );

    /** Stop the command buffer */
    buffer.end();
    /** Submit command buffer to graphics queue */
    vk::SubmitInfo submitInfo( 0, nullptr, nullptr, 1, &buffer, 0, nullptr );
    device.getGraphicsQueue().submit( 1, &submitInfo, vk::Fence() );

    L_TRACE( "({}) stopped and submitted", (void *)&buffer );
    /** Wait will the graphics queue is idle */
    device.getGraphicsQueue().waitIdle();
}