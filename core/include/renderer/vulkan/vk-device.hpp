#pragma once

#include <memory>
#include <SDL2/SDL.h>
#include <vulkan/vulkan.hpp>

#include "vk-instance.hpp"
#include "vk-physicalDevice.hpp"
#include "vk-surface.hpp"

class VulkanDevice
{
private:
    vk::UniqueDevice m_device;
    uint32_t         m_graphicsQueueIndex;
    uint32_t         m_presentQueueIndex;
    vk::Queue        m_graphicsQueue;
    vk::Queue        m_presentQueue;

    struct QueueConfig;
    static QueueConfig getQueueConfig( VulkanPhysicalDevice &physicalDevice,
                                       VulkanSurface        &surface );
protected:
public:
    VulkanDevice( SDL_Window           *window,
                  VulkanPhysicalDevice &physicalDevice,
                  VulkanSurface        &surface );
    ~VulkanDevice();

    vk::Device &getDevice();
    vk::Queue  &getGraphicsQueue();
    uint32_t    getGraphicsQueueIndex();
    vk::Queue  &getPresentQueue();
    uint32_t    getPresentQueueIndex();

    operator vk::Device() { return getDevice(); }
};