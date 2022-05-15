#include <renderer/vulkan/vk-context.hpp>
#include <renderer/vulkan/vk-utils.hpp>
#include <renderer/vulkan/vk-config.hpp>

#include <utils/logging.hpp>

VulkanContext::VulkanContext(SDL_Window *window) : m_instance(window),
                                                   m_physicalDevice(m_instance),
                                                   m_surface(window, m_instance),
                                                   m_device(window, m_physicalDevice, m_surface),
                                                   m_swapchain(window, m_instance, m_physicalDevice, m_surface, m_device)
{
    L_TAG("VulkanContext::VulkanContext");

    L_DEBUG("VulkanContext successfully intiialized");
}

VulkanContext::~VulkanContext()
{
}
