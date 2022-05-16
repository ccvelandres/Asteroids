#pragma once

#include <SDL2/SDL.h>
#include <vulkan/vulkan.hpp>

#include "vk-device.hpp"
#include "vk-swapchain.hpp"

#include <vector>

class VulkanImageView
{
private:
    std::vector<vk::UniqueImageView> m_imageViews;

protected:
public:
    VulkanImageView(VulkanDevice &device,
                    VulkanSwapchain &swapchain);

    ~VulkanImageView();

    vk::ImageView &getImageViews(const int index);
    vk::ImageView &operator[] (std::size_t index) { return getImageViews(index); }
};