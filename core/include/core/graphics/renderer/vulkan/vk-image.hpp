#pragma once

#include <memory>
#include <SDL.h>
#include <vulkan/vulkan.hpp>

#include "vk-commandpool.hpp"
#include "vk-device.hpp"
#include "vk-physicalDevice.hpp"

class VulkanImage
{
private:
    struct Internal;
    std::unique_ptr<Internal> m_internal;

protected:
public:
    VulkanImage(VulkanImage &&o);
    VulkanImage &operator=(VulkanImage &&o);
    VulkanImage(const VulkanPhysicalDevice    &physicalDevice,
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
                const vk::ImageLayout          newLayout);

    uint32_t                getWidth() const;
    uint32_t                getHeight() const;
    uint32_t                getMipLevels() const;
    const vk::Format        getFormat() const;
    const vk::Image        &getImage() const;
    const vk::DeviceMemory &getImageMemory() const;

    ~VulkanImage();
};