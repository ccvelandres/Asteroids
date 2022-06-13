#pragma once

#include <SDL2/SDL.h>
#include <vulkan/vulkan.hpp>

#include "vk-device.hpp"

#include <memory>

class VulkanImageView
{
private:
    struct Internal;
    std::unique_ptr<Internal> m_internal;

protected:
public:
    VulkanImageView(VulkanImageView &&o);
    VulkanImageView &operator=(VulkanImageView &&o);
    VulkanImageView(const VulkanDevice         &device,
                    const vk::Image            &image,
                    const vk::Format           &format,
                    const vk::ImageAspectFlags &aspectFlags,
                    const uint32_t             &mipLevels);
    ~VulkanImageView();

    const vk::ImageView &getImageView() const;
};