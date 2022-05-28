#pragma once

#include <memory>
#include <SDL2/SDL.h>
#include <vulkan/vulkan.hpp>

#include "vk-device.hpp"

class VulkanCommandPool
{
private:
    struct Internal;
    std::unique_ptr<Internal> m_internal;

protected:
public:
    VulkanCommandPool( const VulkanDevice &device );
    ~VulkanCommandPool();

    vk::UniqueCommandBuffer createCommandBuffer( const VulkanDevice &device ) const;
    void endCommandBuffer( vk::CommandBuffer &buffer, const VulkanDevice &device ) const;
};