#pragma once

#include <memory>
#include <SDL2/SDL.h>
#include <vulkan/vulkan.hpp>

#include "vk-device.hpp"
#include "vk-physicalDevice.hpp"

class VulkanImage
{
private:
    uint32_t               m_height;
    uint32_t               m_width;
    uint32_t               m_mipLevels;
    vk::Format             m_format;
    vk::UniqueImage        m_image;
    vk::UniqueDeviceMemory m_imageMemory;

    static vk::UniqueImage createImage( VulkanPhysicalDevice          &physicalDevice,
                                        VulkanDevice                  &device,
                                        const uint32_t                 width,
                                        const uint32_t                 height,
                                        const uint32_t                 mipLevels,
                                        const vk::SampleCountFlagBits &sampleCount,
                                        const vk::Format              &format,
                                        const vk::ImageTiling         &imageTiling,
                                        const vk::ImageUsageFlags     &usageFlags );

    static vk::UniqueDeviceMemory allocateImageMemory( VulkanPhysicalDevice &physicalDevice,
                                                       VulkanDevice         &device,
                                                       const vk::Image      &image,
                                                       const vk::MemoryPropertyFlags &memoryFlags );
protected:
public:
    VulkanImage( VulkanPhysicalDevice          &physicalDevice,
                 VulkanDevice                  &device,
                 const uint32_t                 width,
                 const uint32_t                 height,
                 const uint32_t                 mipLevels,
                 const vk::SampleCountFlagBits &sampleCount,
                 const vk::Format              &format,
                 const vk::ImageTiling         &imageTiling,
                 const vk::ImageUsageFlags     &usageFlags,
                 const vk::MemoryPropertyFlags &memoryFlags );

    uint32_t         getWidth() const;
    uint32_t         getHeight() const;
    uint32_t         getMipLevels() const;
    const vk::Format getFormat() const;
    const vk::Image  getImage() const;

    ~VulkanImage();
};