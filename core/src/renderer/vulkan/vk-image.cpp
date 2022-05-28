#include <renderer/vulkan/vk-image.hpp>
#include <renderer/vulkan/vk-rendercontext.hpp>
#include <renderer/vulkan/vk-utils.hpp>

#include <utils/logging.hpp>

vk::UniqueImage createImage( const VulkanPhysicalDevice    &physicalDevice,
                             const VulkanDevice            &device,
                             const uint32_t                 width,
                             const uint32_t                 height,
                             const uint32_t                 mipLevels,
                             const vk::SampleCountFlagBits &sampleCount,
                             const vk::Format              &format,
                             const vk::ImageTiling         &imageTiling,
                             const vk::ImageUsageFlags     &usageFlags )
{
    L_TAG( "createImage" );

    vk::Extent3D extent( width, height, 1 );

    vk::ImageCreateInfo imageCreateInfo( vk::ImageCreateFlags(),
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
                                         vk::ImageLayout::eUndefined );

    L_TRACE( "Creating UniqueImage" );
    return device.getDevice().createImageUnique( imageCreateInfo );
}

vk::UniqueDeviceMemory allocateImageMemory(
    const VulkanPhysicalDevice    &physicalDevice,
    const VulkanDevice            &device,
    const vk::Image               &image,
    const vk::MemoryPropertyFlags &memoryFlags )
{
    L_TAG( "allocateImageMemory" );

    vk::MemoryRequirements memoryRequirements =
        device.getDevice().getImageMemoryRequirements( image );

    uint32_t memoryTypeIndex =
        physicalDevice.getMemoryTypeIndex( memoryRequirements.memoryTypeBits,
                                           memoryFlags );
    vk::MemoryAllocateInfo info( memoryRequirements.size, memoryTypeIndex );

    L_TRACE( "Allocating image memory" );

    /** Allocate device memory */
    vk::UniqueDeviceMemory deviceMemory(
        device.getDevice().allocateMemoryUnique( info ) );

    /** Bind memory to image */
    device.getDevice().bindImageMemory( image, deviceMemory.get(), 0 );

    return deviceMemory;
}

VulkanImage::VulkanImage( const VulkanPhysicalDevice    &physicalDevice,
                          const VulkanDevice            &device,
                          const uint32_t                 width,
                          const uint32_t                 height,
                          const uint32_t                 mipLevels,
                          const vk::SampleCountFlagBits &sampleCount,
                          const vk::Format              &format,
                          const vk::ImageTiling         &imageTiling,
                          const vk::ImageUsageFlags     &usageFlags,
                          const vk::MemoryPropertyFlags &memoryFlags )
    : m_width( width ),
      m_height( height ),
      m_mipLevels( mipLevels ),
      m_format( format ),
      m_image( ::createImage( physicalDevice,
                              device,
                              width,
                              height,
                              mipLevels,
                              sampleCount,
                              format,
                              imageTiling,
                              usageFlags ) ),
      m_imageMemory(
          ::allocateImageMemory( physicalDevice, device, *m_image, memoryFlags ) )

{
    L_TAG( "VulkanImage::VulkanImage" );
    L_TRACE( "Image allocated and created" );
}

VulkanImage::~VulkanImage() {}

uint32_t                VulkanImage::getWidth() const { return m_width; }
uint32_t                VulkanImage::getHeight() const { return m_height; }
uint32_t                VulkanImage::getMipLevels() const { return m_width; }
const vk::Format        VulkanImage::getFormat() const { return m_format; }
const vk::Image        &VulkanImage::getImage() const { return *m_image; }
const vk::DeviceMemory &VulkanImage::getImageMemory() const
{
    return *m_imageMemory;
};