#include <core/graphics/renderer/vulkan/vk-config.hpp>
#include <core/graphics/renderer/vulkan/vk-context.hpp>
#include <core/graphics/renderer/vulkan/vk-utils.hpp>

#include <utils/logging.hpp>

struct VulkanContext::Internal
{
    SDL_Window *const          window;
    const VulkanInstance       instance;
    const VulkanPhysicalDevice physicalDevice;
    const VulkanSurface        surface;
    const VulkanDevice         device;
    const VulkanCommandPool    commandPool;
    VulkanRenderContext        renderContext;

    Internal(SDL_Window *window)
        : window(window),
          instance(window),
          physicalDevice(instance),
          surface(window, instance),
          device(window, physicalDevice, surface),
          commandPool(device),
          renderContext(window, instance, physicalDevice, device, surface, commandPool)
    {
        L_TAG("VulkanContext::Internal");
        L_TRACE("Internal resources initialized ({})", static_cast<void *>(this));
    }

    ~Internal()
    {
        L_TAG("VulkanContext::~Internal");
        L_TRACE("Internal resources freed ({})", static_cast<void *>(this));
    }

    void recreateRenderContext()
    {
        L_TAG("VulkanContext::Internal::recreateRenderContext");

        /** Wait till the device is idle before recreating render context */
        device.getDevice().waitIdle();
        renderContext = VulkanRenderContext(window, instance, physicalDevice, device, surface, commandPool);
    }

    bool renderBegin()
    {
        L_TAG("VulkanContext::Internal::renderBegin");

        /** Check if render context can begin rendering */
        if (renderContext.renderBegin(device)) return true;

        /** Recreate render context if it isn't valid */
        L_TRACE("Could not begin renderContext, recreating...");
        recreateRenderContext();
        return false;
    }

    void renderEnd()
    {
        L_TAG("VulkanContext::Internal::renderEnd");

        /** Check if render context can begin rendering */
        if (!renderContext.renderEnd(device))
        {
            /** Recreate render context if it isn't valid */
            L_TRACE("Could not end rendering, recreating...");
            recreateRenderContext();
        }
    }
};

VulkanContext::VulkanContext(SDL_Window *window) : m_internal(std::make_unique<Internal>(window)) {}

VulkanContext::VulkanContext(VulkanContext &&o)            = default;
VulkanContext &VulkanContext::operator=(VulkanContext &&o) = default;
VulkanContext::~VulkanContext()                            = default;

bool VulkanContext::renderBegin() const { return m_internal->renderBegin(); }

void VulkanContext::renderEnd() const { m_internal->renderEnd(); }
