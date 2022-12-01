#include <core/graphics/renderer/vulkan/vk-config.hpp>
#include <core/graphics/renderer/vulkan/vk-debug.hpp>
#include <core/graphics/renderer/vulkan/vk-instance.hpp>
#include <core/graphics/renderer/vulkan/vk-utils.hpp>

#include <utils/logging.hpp>

std::vector<const char *> getValidationLayers()
{
    L_TAG("getValidationLayers");

    std::vector<const char *>        desiredValidationLayers;
    std::vector<vk::LayerProperties> availableLayers = vk::enumerateInstanceLayerProperties();
    for (const auto &layer : VulkanConfig::getValidationLayers())
    {
        if (VulkanUtils::checkValidationLayerSupport(layer, availableLayers))
        {
            desiredValidationLayers.push_back(layer);
            L_DEBUG("ValidationLayer requested: {}", layer);
        }
        else
        {
            L_WARN("ValidationLayer not available: {}", layer);
        }
    }

    return desiredValidationLayers;
}

std::vector<const char *> getRequiredExtensions(SDL_Window *window)
{
    L_TAG("getRequiredExtensions");

    std::vector<const char *> extensionNames;
    {
        unsigned int              extensionCount = 0;
        std::vector<const char *> sdlExtensions;
        SDL_Vulkan_GetInstanceExtensions(window, &extensionCount, nullptr);
        sdlExtensions.reserve(extensionCount);
        SDL_Vulkan_GetInstanceExtensions(window, &extensionCount, sdlExtensions.data());
        for (int i = 0; i < extensionCount; i++)
        {
            L_DEBUG("Adding SDLRequiredInstanceExtension: {}", sdlExtensions[i]);
            extensionNames.push_back(sdlExtensions[i]);
        }
    }

    for (const auto &extension : VulkanConfig::getInstanceExtensions())
    {
        extensionNames.push_back(extension);
    }

    return extensionNames;
}

vk::UniqueInstance createInstance(SDL_Window *window)
{
    L_TAG("createInstance");

    uint32_t version = vk::enumerateInstanceVersion();
    L_DEBUG("Vulkan API Version: {}.{}.{}",
            VK_VERSION_MAJOR(version),
            VK_VERSION_MINOR(version),
            VK_VERSION_PATCH(version));

    /** Check if validation layers are available and create a desired layers */
    std::vector<const char *> desiredValidationLayers = getValidationLayers();

    /** Get Instance extensions */
    std::vector<const char *> extensionNames = getRequiredExtensions(window);

    vk::InstanceCreateInfo instanceCreateInfo(vk::InstanceCreateFlags(),
                                              VulkanConfig::getApplicationInfo(),
                                              static_cast<uint32_t>(desiredValidationLayers.size()),
                                              desiredValidationLayers.data(),
                                              static_cast<uint32_t>(extensionNames.size()),
                                              extensionNames.data());

    return vk::createInstanceUnique(instanceCreateInfo);
}

struct VulkanInstance::Internal
{
    const vk::UniqueInstance instance;
    const VulkanDebugUtils   debugUtils;

    Internal(SDL_Window *window) : instance(::createInstance(window)), debugUtils(*instance)
    {
        L_TAG("VulkanInstance::Internal");
        L_TRACE("Internal resources initialized ({})", static_cast<void *>(this));
    }

    ~Internal()
    {
        L_TAG("VulkanInstance::~Internal");
        L_TRACE("Internal resources freed ({})", static_cast<void *>(this));
    }
};

VulkanInstance::VulkanInstance(SDL_Window *window) : m_internal(std::make_unique<Internal>(window)) {}

VulkanInstance::VulkanInstance(VulkanInstance &&o)            = default;
VulkanInstance &VulkanInstance::operator=(VulkanInstance &&o) = default;
VulkanInstance::~VulkanInstance()                             = default;

const vk::Instance &VulkanInstance::getInstance() const { return *m_internal->instance; }