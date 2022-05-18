#include <renderer/vulkan/vk-imageview.hpp>
#include <renderer/vulkan/vk-utils.hpp>
#include <SDL2/SDL_vulkan.h>

#include <utils/logging.hpp>

#include <algorithm>
#include <limits>

VulkanImageView::VulkanImageView(VulkanDevice &device,
                                 VulkanSwapchain &swapchain)
{
    L_TAG("VulkanImageView::VulkanImageView");

    vk::ImageSubresourceRange imageSubresourceRange(vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1);
    vk::ComponentMapping componentMapping(vk::ComponentSwizzle::eIdentity,
                                          vk::ComponentSwizzle::eIdentity,
                                          vk::ComponentSwizzle::eIdentity,
                                          vk::ComponentSwizzle::eIdentity);

    for (const auto &image : swapchain.getImages())
    {
        vk::ImageViewCreateInfo imageViewCreateInfo = {};
        imageViewCreateInfo.setImage(image)
            .setViewType(vk::ImageViewType::e2D)
            .setFormat(swapchain.getFormat().format)
            .setComponents(componentMapping)
            .setSubresourceRange(imageSubresourceRange);

        m_imageViews.push_back(device.getDevice().createImageViewUnique(imageViewCreateInfo));
    }
}

VulkanImageView::~VulkanImageView()
{
}

vk::ImageView &VulkanImageView::getImageViews(const int index)
{
    L_TAG("VulkanImageView::getImageViews");

    if (index >= m_imageViews.size())
    {
        L_THROW(std::out_of_range, "MaxIndex: {} Argument: {}", m_imageViews.size(), index);
    }
    return *m_imageViews[index];
}
