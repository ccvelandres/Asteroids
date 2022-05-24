#pragma once

#include <memory>
#include <SDL2/SDL.h>
#include <vulkan/vulkan.hpp>

#include "vk-instance.hpp"

class VulkanSurface
{
private:
    vk::UniqueSurfaceKHR m_surface;

protected:
public:
    VulkanSurface( SDL_Window *window, VulkanInstance &instance );
    ~VulkanSurface();

    vk::SurfaceKHR &getSurface();

    operator vk::SurfaceKHR() { return getSurface(); }
};