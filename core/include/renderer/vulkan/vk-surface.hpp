#pragma once

#include <SDL2/SDL.h>
#include <vulkan/vulkan.hpp>
#include <memory>

class VulkanSurface
{
private:
    vk::UniqueSurfaceKHR m_surface;

protected:
public:
    VulkanSurface(SDL_Window *window,
                  const vk::Instance &instance);
    ~VulkanSurface();

    vk::SurfaceKHR &getSurface();
    operator vk::SurfaceKHR() { return getSurface(); }
};