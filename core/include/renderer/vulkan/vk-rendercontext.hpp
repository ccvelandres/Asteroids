#pragma once

#include <SDL2/SDL.h>
#include <vulkan/vulkan.hpp>

#include "vk-device.hpp"
#include "vk-imageview.hpp"
#include "vk-instance.hpp"
#include "vk-physicalDevice.hpp"
#include "vk-renderpass.hpp"
#include "vk-surface.hpp"
#include "vk-swapchain.hpp"

class VulkanRenderContext
{
private:
    VulkanSwapchain  m_swapchain;
    VulkanImageView  m_imageview;
    VulkanRenderPass m_renderpass;

protected:
public:
    VulkanRenderContext( SDL_Window           *window,
                         VulkanInstance       &instance,
                         VulkanPhysicalDevice &physicalDevice,
                         VulkanDevice         &device,
                         VulkanSurface        &surface );
    ~VulkanRenderContext();
};