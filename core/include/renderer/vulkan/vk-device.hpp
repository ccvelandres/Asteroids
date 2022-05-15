#pragma once

#include <SDL2/SDL.h>
#include <vulkan/vulkan.hpp>
#include <memory>

class VulkanDevice
{
private:
    vk::UniqueDevice m_device;
    uint32_t m_graphicsQueueIndex;
    uint32_t m_presentQueueIndex;
    vk::Queue m_graphicsQueue;
    vk::Queue m_presentQueue;

    struct QueueConfig;
    static QueueConfig getQueueConfig(const vk::PhysicalDevice &physicalDevice,
                                      const vk::SurfaceKHR &surface);
protected:
public:
    VulkanDevice(SDL_Window *window,
                 const vk::Instance &instance,
                 const vk::PhysicalDevice &physicalDevice,
                 const vk::SurfaceKHR &surface);
    ~VulkanDevice();

    vk::Device &getDevice();
    operator vk::Device() { return getDevice(); }
    
    vk::Queue &getGraphicsQueue();
    uint32_t getGraphicsQueueIndex();
    vk::Queue &getPresentQueue();
    uint32_t getPresentQueueIndex();
};