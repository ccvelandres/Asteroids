#pragma once

#include <SDL2/SDL.h>
#include <vulkan/vulkan.hpp>

#include "vk-device.hpp"
#include "vk-instance.hpp"
#include "vk-physicalDevice.hpp"
#include "vk-surface.hpp"
#include "vk-swapchain.hpp"
#include "vk-imageview.hpp"

class VulkanRenderContext
{
private:
    VulkanSwapchain m_swapchain;
    VulkanImageView m_imageview;
    
protected:
public:
    VulkanRenderContext(SDL_Window *window,
                        VulkanInstance &instance,
                        VulkanPhysicalDevice &physicalDevice,
                        VulkanDevice &device,
                        VulkanSurface &surface);
    ~VulkanRenderContext();
};