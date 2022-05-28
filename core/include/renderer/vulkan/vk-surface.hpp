#pragma once

#include <memory>
#include <SDL2/SDL.h>
#include <vulkan/vulkan.hpp>

#include "vk-instance.hpp"

class VulkanSurface
{
private:
    const vk::UniqueSurfaceKHR m_surface;

protected:
public:
    VulkanSurface( SDL_Window *window, const VulkanInstance &instance );
    ~VulkanSurface();

    const vk::SurfaceKHR &getSurface() const;

    operator vk::SurfaceKHR() { return getSurface(); }
};