#pragma once

#include <memory>
#include <SDL.h>
#include <vector>
#include <vulkan/vulkan.hpp>

#include "vk-device.hpp"

class VulkanCommandPool
{
private:
    struct Internal;
    std::unique_ptr<Internal> m_internal;

protected:
public:
    VulkanCommandPool(VulkanCommandPool &&o);
    VulkanCommandPool &operator=(VulkanCommandPool &&o);
    VulkanCommandPool(const VulkanDevice &device);
    ~VulkanCommandPool();

    std::vector<vk::UniqueCommandBuffer> createCommandBuffers(const VulkanDevice &device, const uint32_t count) const;
    vk::UniqueCommandBuffer              createCommandBuffer(const VulkanDevice &device) const;
    void                                 endCommandBuffer(vk::CommandBuffer &buffer, const VulkanDevice &device) const;
};