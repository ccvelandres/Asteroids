#include <core/graphics/renderer/vulkan/vk-config.hpp>
#include <core/graphics/renderer/vulkan/vk-context.hpp>
#include <core/graphics/renderer/vulkan/vk-renderer.hpp>
#include <core/graphics/renderer/vulkan/vk-utils.hpp>

#include <utils/logging.hpp>

#include <exception>

struct VulkanRenderer::Internal
{
    const SDL_Window *const window;
    const VulkanContext     context;

    Internal(SDL_Window *window) : window(window), context(window)
    {
        L_TAG("VulkanRenderer::Internal");
        L_TRACE("Internal resources initialized ({})", static_cast<void *>(this));
    }

    ~Internal()
    {
        L_TAG("VulkanRenderer::~Internal");
        L_TRACE("Internal resources freed ({})", static_cast<void *>(this));
    }
};

VulkanRenderer::VulkanRenderer(SDL_Window *window) : m_internal(std::make_unique<Internal>(window)) {}

VulkanRenderer::VulkanRenderer(VulkanRenderer &&o)            = default;
VulkanRenderer &VulkanRenderer::operator=(VulkanRenderer &&o) = default;
VulkanRenderer::~VulkanRenderer()                             = default;

void VulkanRenderer::init() {}

void VulkanRenderer::update(const time_ms delta) {}

void VulkanRenderer::clean() {}

void VulkanRenderer::refresh() {}

bool VulkanRenderer::renderBegin() { return m_internal->context.renderBegin(); }

void VulkanRenderer::render() {}

void VulkanRenderer::renderEnd() { m_internal->context.renderEnd(); }
