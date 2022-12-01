#include <core/graphics/renderer/vulkan/vk-commandpool.hpp>
#include <core/graphics/renderer/vulkan/vk-renderpass.hpp>
#include <core/graphics/renderer/vulkan/vk-utils.hpp>

#include <utils/logging.hpp>

vk::UniqueCommandPool createCommandPool(const VulkanDevice &device)
{
    L_TAG("createCommandPool");

    /** Create a command pool for the device */
    vk::CommandPoolCreateInfo commandPoolCreateInfo(vk::CommandPoolCreateFlagBits::eResetCommandBuffer, device.getGraphicsQueueIndex());

    return device.getDevice().createCommandPoolUnique(commandPoolCreateInfo);
}

struct VulkanCommandPool::Internal
{
    const vk::UniqueCommandPool commandPool;

    Internal(const VulkanDevice &device) : commandPool(::createCommandPool(device))
    {
        L_TAG("VulkanCommandPool::Internal");
        L_TRACE("Internal resources initialized ({})", static_cast<void *>(this));
    }

    ~Internal()
    {
        L_TAG("VulkanCommandPool::~Internal");
        L_TRACE("Internal resources freed ({})", static_cast<void *>(this));
    }
};

VulkanCommandPool::VulkanCommandPool(const VulkanDevice &device) : m_internal(std::make_unique<Internal>(device)) {}

VulkanCommandPool::VulkanCommandPool(VulkanCommandPool &&o)            = default;
VulkanCommandPool &VulkanCommandPool::operator=(VulkanCommandPool &&o) = default;
VulkanCommandPool::~VulkanCommandPool()                                = default;

std::vector<vk::UniqueCommandBuffer> VulkanCommandPool::createCommandBuffers(const VulkanDevice &device,
                                                                             const uint32_t      count) const
{
    L_TAG("VulkanCommandPool::createCommandBuffers");

    vk::CommandBufferAllocateInfo commandBufferAllocateInfo(*m_internal->commandPool,
                                                            vk::CommandBufferLevel::ePrimary,
                                                            count);

    return device.getDevice().allocateCommandBuffersUnique(commandBufferAllocateInfo);
}

vk::UniqueCommandBuffer VulkanCommandPool::createCommandBuffer(const VulkanDevice &device) const
{
    L_TAG("VulkanCommandPool::createCommandBuffer");

    vk::UniqueCommandBuffer buffer(std::move(createCommandBuffers(device, 1)[0]));

    vk::CommandBufferBeginInfo beginInfo(vk::CommandBufferUsageFlagBits::eOneTimeSubmit, nullptr);

    /** Request to begin the buffer */
    buffer->begin(beginInfo);
    L_TRACE("CommandBuffer created and started ({})", (void *)&buffer.get());
    return buffer;
}

void VulkanCommandPool::endCommandBuffer(vk::CommandBuffer &buffer, const VulkanDevice &device) const
{
    L_TAG("VulkanCommandPool::endCommandBuffer");

    /** Stop the command buffer */
    buffer.end();
    /** Submit command buffer to graphics queue */
    vk::SubmitInfo submitInfo(0, nullptr, nullptr, 1, &buffer, 0, nullptr);
    device.getGraphicsQueue().submit(1, &submitInfo, vk::Fence());

    L_TRACE("CommandBuffer stopped and submitted ({})", (void *)&buffer);
    /** Wait will the graphics queue is idle */
    device.getGraphicsQueue().waitIdle();
}