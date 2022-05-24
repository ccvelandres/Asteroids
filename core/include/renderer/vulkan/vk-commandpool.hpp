#pragma once

#include <memory>
#include <SDL2/SDL.h>
#include <vulkan/vulkan.hpp>

#include "vk-device.hpp"

class VulkanCommandPool
{
private:
    vk::UniqueCommandPool m_commandPool;
    VulkanDevice         &m_device;

    static vk::UniqueCommandPool createCommandPool( vk::Device &device, uint32_t queueIndex );
protected:
public:
    VulkanCommandPool( VulkanDevice &device );
    ~VulkanCommandPool();

    vk::UniqueCommandBuffer createCommandBuffer( );
    void endCommandBuffer( vk::CommandBuffer &commandBuffer );
};