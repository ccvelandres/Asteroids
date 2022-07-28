#include <core/graphics/renderer/vulkan/vk-image.hpp>
#include <core/graphics/renderer/vulkan/vk-rendercontext.hpp>
#include <core/graphics/renderer/vulkan/vk-utils.hpp>

#include <utils/logging.hpp>

void transitionImageLayout(const VulkanDevice      &device,
                           const VulkanCommandPool &commandPool,
                           const vk::Image         &image,
                           const vk::Format        &format,
                           const uint32_t          &mipLevels,
                           const vk::ImageLayout    oldLayout,
                           const vk::ImageLayout    newLayout)
{
    L_TAG("transitionImageLayout");
    vk::UniqueCommandBuffer commandBuffer = commandPool.createCommandBuffer(device);

    vk::ImageMemoryBarrier imageBarrier;
    imageBarrier.image                           = image;
    imageBarrier.oldLayout                       = oldLayout;
    imageBarrier.newLayout                       = newLayout;
    imageBarrier.subresourceRange.aspectMask     = vk::ImageAspectFlagBits::eColor;
    imageBarrier.subresourceRange.baseMipLevel   = 0;
    imageBarrier.subresourceRange.levelCount     = mipLevels;
    imageBarrier.subresourceRange.baseArrayLayer = 0;
    imageBarrier.subresourceRange.layerCount     = 1;

    if (oldLayout == vk::ImageLayout::eUndefined && newLayout == vk::ImageLayout::eColorAttachmentOptimal)
    {
        imageBarrier.dstAccessMask = vk::AccessFlagBits::eColorAttachmentRead | vk::AccessFlagBits::eColorAttachmentWrite;
        L_TRACE("Sending transition barrier for eUndefined -> eColorAttachmentOptimal");
        /** Send the image barrier to pipeline stage */
        commandBuffer->pipelineBarrier(vk::PipelineStageFlagBits::eTopOfPipe,
                                       vk::PipelineStageFlagBits::eColorAttachmentOutput,
                                       vk::DependencyFlags(),
                                       0,
                                       nullptr,
                                       0,
                                       nullptr,
                                       1,
                                       &imageBarrier);
    }
    else if (oldLayout == vk::ImageLayout::eUndefined && newLayout == vk::ImageLayout::eDepthStencilAttachmentOptimal)
    {
        imageBarrier.dstAccessMask =
            vk::AccessFlagBits::eDepthStencilAttachmentRead | vk::AccessFlagBits::eDepthStencilAttachmentWrite;
        imageBarrier.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eDepth;

        L_TRACE("Sending transition barrier for eUndefined -> eDepthStencilAttachmentOptimal");
        commandBuffer->pipelineBarrier(vk::PipelineStageFlagBits::eTopOfPipe,
                                       vk::PipelineStageFlagBits::eEarlyFragmentTests,
                                       vk::DependencyFlags(),
                                       0,
                                       nullptr,
                                       0,
                                       nullptr,
                                       1,
                                       &imageBarrier);
    }
    else
    {
        L_THROW_LOGIC("Unsupported old -> new layout transition");
    }

    /** Close and send the command buffer */
    commandPool.endCommandBuffer(*commandBuffer, device);
}

vk::UniqueImage createImage(const VulkanPhysicalDevice    &physicalDevice,
                            const VulkanDevice            &device,
                            const uint32_t                 width,
                            const uint32_t                 height,
                            const uint32_t                 mipLevels,
                            const vk::SampleCountFlagBits &sampleCount,
                            const vk::Format              &format,
                            const vk::ImageTiling         &imageTiling,
                            const vk::ImageUsageFlags     &usageFlags)
{
    L_TAG("createImage");

    vk::Extent3D extent(width, height, 1);

    vk::ImageCreateInfo imageCreateInfo(vk::ImageCreateFlags(),
                                        vk::ImageType::e2D,
                                        format,
                                        extent,
                                        mipLevels,
                                        1,
                                        sampleCount,
                                        imageTiling,
                                        usageFlags,
                                        vk::SharingMode::eExclusive,
                                        0,
                                        nullptr,
                                        vk::ImageLayout::eUndefined);

    L_TRACE("Creating UniqueImage");
    return device.getDevice().createImageUnique(imageCreateInfo);
}

vk::UniqueDeviceMemory allocateImageMemory(const VulkanPhysicalDevice    &physicalDevice,
                                           const VulkanDevice            &device,
                                           const vk::Image               &image,
                                           const vk::MemoryPropertyFlags &memoryFlags)
{
    L_TAG("allocateImageMemory");

    vk::MemoryRequirements memoryRequirements = device.getDevice().getImageMemoryRequirements(image);

    uint32_t memoryTypeIndex = physicalDevice.getMemoryTypeIndex(memoryRequirements.memoryTypeBits, memoryFlags);
    vk::MemoryAllocateInfo info(memoryRequirements.size, memoryTypeIndex);

    L_TRACE("Allocating image memory");

    /** Allocate device memory */
    vk::UniqueDeviceMemory deviceMemory(device.getDevice().allocateMemoryUnique(info));

    /** Bind memory to image */
    device.getDevice().bindImageMemory(image, deviceMemory.get(), 0);

    return deviceMemory;
}

struct VulkanImage::Internal
{
    const uint32_t               height;
    const uint32_t               width;
    const uint32_t               mipLevels;
    const vk::Format             format;
    const vk::UniqueImage        image;
    const vk::UniqueDeviceMemory imageMemory;

    Internal(const VulkanPhysicalDevice    &physicalDevice,
             const VulkanDevice            &device,
             const VulkanCommandPool       &commandPool,
             const uint32_t                 width,
             const uint32_t                 height,
             const uint32_t                 mipLevels,
             const vk::SampleCountFlagBits &sampleCount,
             const vk::Format              &format,
             const vk::ImageTiling         &imageTiling,
             const vk::ImageUsageFlags     &usageFlags,
             const vk::MemoryPropertyFlags &memoryFlags,
             const vk::ImageLayout          oldLayout,
             const vk::ImageLayout          newLayout)
        : width(width),
          height(height),
          mipLevels(mipLevels),
          format(format),
          image(
              ::createImage(physicalDevice, device, width, height, mipLevels, sampleCount, format, imageTiling, usageFlags)),
          imageMemory(::allocateImageMemory(physicalDevice, device, *image, memoryFlags))
    {
        L_TAG("VulkanImage::Internal");
        ::transitionImageLayout(device, commandPool, *image, format, mipLevels, oldLayout, newLayout);
        L_TRACE("Internal resources initialized ({})", static_cast<void *>(this));
    }

    ~Internal()
    {
        L_TAG("VulkanImage::~Internal");
        L_TRACE("Internal resources freed ({})", static_cast<void *>(this));
    }
};

VulkanImage::VulkanImage(const VulkanPhysicalDevice    &physicalDevice,
                         const VulkanDevice            &device,
                         const VulkanCommandPool       &commandPool,
                         const uint32_t                 width,
                         const uint32_t                 height,
                         const uint32_t                 mipLevels,
                         const vk::SampleCountFlagBits &sampleCount,
                         const vk::Format              &format,
                         const vk::ImageTiling         &imageTiling,
                         const vk::ImageUsageFlags     &usageFlags,
                         const vk::MemoryPropertyFlags &memoryFlags,
                         const vk::ImageLayout          oldLayout,
                         const vk::ImageLayout          newLayout)
    : m_internal(std::make_unique<Internal>(physicalDevice,
                                            device,
                                            commandPool,
                                            width,
                                            height,
                                            mipLevels,
                                            sampleCount,
                                            format,
                                            imageTiling,
                                            usageFlags,
                                            memoryFlags,
                                            oldLayout,
                                            newLayout))

{
}

VulkanImage::VulkanImage(VulkanImage &&o)            = default;
VulkanImage &VulkanImage::operator=(VulkanImage &&o) = default;
VulkanImage::~VulkanImage()                          = default;

uint32_t                VulkanImage::getWidth() const { return m_internal->width; }
uint32_t                VulkanImage::getHeight() const { return m_internal->height; }
uint32_t                VulkanImage::getMipLevels() const { return m_internal->width; }
const vk::Format        VulkanImage::getFormat() const { return m_internal->format; }
const vk::Image        &VulkanImage::getImage() const { return *m_internal->image; }
const vk::DeviceMemory &VulkanImage::getImageMemory() const { return *m_internal->imageMemory; };