#include <renderer/vulkan/vk-renderer.hpp>
#include <renderer/vulkan/vk-config.hpp>
#include <renderer/vulkan/vk-utils.hpp>

#include <utils/logging.hpp>

#include <exception>

VulkanRenderer::VulkanRenderer(SDL_Window *window) : 
    m_window(window),
    context(new VulkanContext(m_window))
{
    L_TAG("VulkanRenderer::VulkanRenderer");

}

VulkanRenderer::~VulkanRenderer()
{
    delete context;
}