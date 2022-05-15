#pragma once

#include <SDL2/SDL.h>
#include <vulkan/vulkan.hpp>
#include <memory> 

class VulkanSurface {
private:
    vk::UniqueSurfaceKHR m_surface;
protected:
public:
    VulkanSurface(const vk::Instance &instance, SDL_Window *window);

    vk::SurfaceKHR& getSurface();
    operator vk::SurfaceKHR() { return getSurface(); }
};