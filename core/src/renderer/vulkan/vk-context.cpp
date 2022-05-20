#include <renderer/vulkan/vk-config.hpp>
#include <renderer/vulkan/vk-context.hpp>
#include <renderer/vulkan/vk-utils.hpp>

#include <utils/logging.hpp>

VulkanContext::VulkanContext(SDL_Window *window) : m_instance(window),
                                                   m_physicalDevice(m_instance),
                                                   m_surface(window, m_instance),
                                                   m_device(window, m_physicalDevice, m_surface),
                                                   m_renderContext(window, m_instance, m_physicalDevice, m_device, m_surface)
{
    L_TAG("VulkanContext::VulkanContext");

    L_DEBUG("VulkanContext successfully intiialized");
}

VulkanContext::~VulkanContext()
{
}
