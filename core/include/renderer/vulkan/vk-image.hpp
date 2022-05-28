#pragma once

#include <memory>
#include <SDL2/SDL.h>
#include <vulkan/vulkan.hpp>

#include "vk-device.hpp"
#include "vk-physicalDevice.hpp"

class VulkanImage
{
private:

    const uint32_t               m_height;
    const uint32_t               m_width;
    const uint32_t               m_mipLevels;
    const vk::Format             m_format;
    const vk::UniqueImage        m_image;
    const vk::UniqueDeviceMemory m_imageMemory;

protected:
public:
    VulkanImage( const VulkanPhysicalDevice    &physicalDevice,
                 const VulkanDevice            &device,
                 const uint32_t                 width,
                 const uint32_t                 height,
                 const uint32_t                 mipLevels,
                 const vk::SampleCountFlagBits &sampleCount,
                 const vk::Format              &format,
                 const vk::ImageTiling         &imageTiling,
                 const vk::ImageUsageFlags     &usageFlags,
                 const vk::MemoryPropertyFlags &memoryFlags );

    uint32_t                getWidth() const;
    uint32_t                getHeight() const;
    uint32_t                getMipLevels() const;
    const vk::Format        getFormat() const;
    const vk::Image        &getImage() const;
    const vk::DeviceMemory &getImageMemory() const;

    ~VulkanImage();
};