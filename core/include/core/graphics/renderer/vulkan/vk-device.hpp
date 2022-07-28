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
    struct Internal;
    std::unique_ptr<Internal> m_internal;

protected:
public:
    VulkanDevice(VulkanDevice &&o);
    VulkanDevice &operator=(VulkanDevice &&o);
    VulkanDevice(SDL_Window *window, const VulkanPhysicalDevice &physicalDevice, const VulkanSurface &surface);
    ~VulkanDevice();

    const vk::Device                &getDevice() const;
    const vk::Queue                 &getGraphicsQueue() const;
    const uint32_t                   getGraphicsQueueIndex() const;
    const vk::Queue                 &getPresentQueue() const;
    const uint32_t                   getPresentQueueIndex() const;
    const bool                       hasDiscreteQueue() const;
    std::vector<vk::UniqueSemaphore> createSemaphores(const uint32_t count) const;
    std::vector<vk::UniqueFence>     createFences(const uint32_t count) const;

    operator vk::Device() { return getDevice(); }
};