#pragma once

#include <vulkan/vulkan.hpp>

#include "vk-device.hpp"
#include "vk-instance.hpp"
#include "vk-physicalDevice.hpp"
#include "vk-rendercontext.hpp"
#include "vk-surface.hpp"

class VulkanContext
{
private:
    VulkanInstance       m_instance;
    VulkanPhysicalDevice m_physicalDevice;
    VulkanSurface        m_surface;
    VulkanDevice         m_device;
    VulkanRenderContext  m_renderContext;
protected:
public:
    VulkanContext( SDL_Window *window );
    ~VulkanContext();
};