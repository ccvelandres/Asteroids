#pragma once

#include <memory>
#include <SDL.h>
#include <vulkan/vulkan.hpp>

#include "vk-instance.hpp"

class VulkanSurface
{
private:
    struct Internal;
    std::unique_ptr<Internal> m_internal;

protected:
public:
    VulkanSurface(VulkanSurface &&o);
    VulkanSurface &operator=(VulkanSurface &&o);
    VulkanSurface(SDL_Window *window, const VulkanInstance &instance);
    ~VulkanSurface();

    const vk::SurfaceKHR &getSurface() const;

    operator vk::SurfaceKHR() { return getSurface(); }
};