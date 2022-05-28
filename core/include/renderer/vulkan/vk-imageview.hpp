#pragma once

#include <SDL2/SDL.h>
#include <vulkan/vulkan.hpp>

#include "vk-device.hpp"
#include "vk-swapchain.hpp"

#include <memory>
#include <vector>

class VulkanImageView
{
private:
    struct Internal;
    std::unique_ptr<Internal> m_internal;

protected:
public:
    VulkanImageView( const VulkanDevice    &device,
                     const VulkanSwapchain &swapchain );

    ~VulkanImageView();

    const vk::ImageView &getImageViews( const int index ) const;

    const vk::ImageView &operator[]( std::size_t index ) const
    {
        return getImageViews( index );
    }
};