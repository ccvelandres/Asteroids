#include <core/graphics/renderer/vulkan/vk-imageview.hpp>
#include <core/graphics/renderer/vulkan/vk-utils.hpp>
#include <SDL_vulkan.h>

#include <utils/logging.hpp>

#include <algorithm>
#include <limits>

vk::UniqueImageView createImageViews(const VulkanDevice         &device,
                                     const vk::Image            &image,
                                     const vk::Format           &format,
                                     const vk::ImageAspectFlags &aspectFlags,
                                     const uint32_t             &mipLevels)
{
    L_TAG("createImageViews");

    vk::ImageSubresourceRange imageSubresourceRange(vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1);
    vk::ComponentMapping      componentMapping(vk::ComponentSwizzle::eIdentity,
                                          vk::ComponentSwizzle::eIdentity,
                                          vk::ComponentSwizzle::eIdentity,
                                          vk::ComponentSwizzle::eIdentity);

    /** Create an image view for image */
    vk::ImageViewCreateInfo imageViewCreateInfo(vk::ImageViewCreateFlags(),
                                                image,
                                                vk::ImageViewType::e2D,
                                                format,
                                                componentMapping,
                                                imageSubresourceRange);

    return device.getDevice().createImageViewUnique(imageViewCreateInfo);
}

struct VulkanImageView::Internal
{
    const vk::UniqueImageView imageView;

    Internal(const VulkanDevice         &device,
             const vk::Image            &image,
             const vk::Format           &format,
             const vk::ImageAspectFlags &aspectFlags,
             const uint32_t             &mipLevels)
        : imageView(::createImageViews(device, image, format, aspectFlags, mipLevels))
    {
        L_TAG("VulkanImageView::Internal");
        L_TRACE("Internal resources initialized ({})", static_cast<void *>(this));
    }

    ~Internal()
    {
        L_TAG("VulkanImageView::~Internal");
        L_TRACE("Internal resources freed ({})", static_cast<void *>(this));
    }
};

VulkanImageView::VulkanImageView(const VulkanDevice         &device,
                                 const vk::Image            &image,
                                 const vk::Format           &format,
                                 const vk::ImageAspectFlags &aspectFlags,
                                 const uint32_t             &mipLevels)
    : m_internal(std::make_unique<Internal>(device, image, format, aspectFlags, mipLevels))
{
}

VulkanImageView::VulkanImageView(VulkanImageView &&o)            = default;
VulkanImageView &VulkanImageView::operator=(VulkanImageView &&o) = default;
VulkanImageView::~VulkanImageView()                              = default;

const vk::ImageView &VulkanImageView::getImageView() const
{
    L_TAG("VulkanImageView::getImageView");
    return *m_internal->imageView;
}
