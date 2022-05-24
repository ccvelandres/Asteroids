#pragma once

#include <renderer.hpp>
#include <vulkan/vulkan.hpp>

#include "vk-context.hpp"

class VulkanRenderer : public BaseRenderer
{
private:
    SDL_Window *const m_window;
    VulkanContext    *context;

protected:
public:
    VulkanRenderer( SDL_Window *window );
    ~VulkanRenderer();
};