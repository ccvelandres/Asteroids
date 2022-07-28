#include <core/graphics/renderer/vulkan/vk-debug.hpp>

#include <utils/logging.hpp>

#include <iostream>

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT      messageSeverity,
                                                    VkDebugUtilsMessageTypeFlagsEXT             messageType,
                                                    const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
                                                    void                                       *pUserData)
{
    if (messageSeverity
        == static_cast<VkDebugUtilsMessageSeverityFlagBitsEXT>(vk::DebugUtilsMessageSeverityFlagBitsEXT::eError))
    {
        logging::error("[vk-debug]: {}", pCallbackData->pMessage);
    }
    else if (messageSeverity
             == static_cast<VkDebugUtilsMessageSeverityFlagBitsEXT>(vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning))
    {
        logging::warn("[vk-debug]: {}", pCallbackData->pMessage);
    }
    else if (messageSeverity
             == static_cast<VkDebugUtilsMessageSeverityFlagBitsEXT>(vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo))
    {
        logging::info("[vk-debug]: {}", pCallbackData->pMessage);
    }
    else if (messageSeverity
             == static_cast<VkDebugUtilsMessageSeverityFlagBitsEXT>(vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose))
    {
        logging::trace("[vk-debug]: {}", pCallbackData->pMessage);
    }

    return VK_FALSE;
}

vk::DebugUtilsMessengerEXT createDebugUtilsMessenger(const vk::Instance              &instance,
                                                     const vk::DispatchLoaderDynamic &dispatcher)
{
    vk::DebugUtilsMessengerCreateInfoEXT createInfo(
        vk::DebugUtilsMessengerCreateFlagsEXT(),
        vk::DebugUtilsMessageSeverityFlagBitsEXT::eError | vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose
            | vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning,
        vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance
            | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation,
        debugCallback,
        nullptr);

    // PFN_vkCreateDebugUtilsMessengerEXT fn = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(
    //     vkGetInstanceProcAddr( instance, "vkCreateDebugUtilsMessengerEXT" ) );

    // fn(instance, )
    // auto d = vk::DispatchLoaderDynamic(instance, vkGetInstanceProcAddr);
    // vk::DispatchLoaderDynamic d(instance, vkGetInstanceProcAddr);
    return instance.createDebugUtilsMessengerEXT(createInfo, nullptr, dispatcher);
}

struct VulkanDebugUtils::Internal
{
    const vk::Instance              &instance;
    const vk::DispatchLoaderDynamic  dispatcher;
    const vk::DebugUtilsMessengerEXT debugUtilsMessenger;

    Internal(const vk::Instance &instance)
        : instance(instance),
          dispatcher(instance, vkGetInstanceProcAddr),
          debugUtilsMessenger(::createDebugUtilsMessenger(instance, dispatcher))
    {
        L_TAG("VulkanDebugUtils::Internal");
        L_TRACE("Internal resources initialized ({})", static_cast<void *>(this));
    }

    ~Internal()
    {
        L_TAG("VulkanDebugUtils::~Internal");
        instance.destroyDebugUtilsMessengerEXT(debugUtilsMessenger, nullptr, dispatcher);
        L_TRACE("Internal resources freed ({})", static_cast<void *>(this));
    }
};

VulkanDebugUtils::VulkanDebugUtils(const vk::Instance &instance) : m_internal(std::make_unique<Internal>(instance)) {}

VulkanDebugUtils::VulkanDebugUtils(VulkanDebugUtils &&o)            = default;
VulkanDebugUtils &VulkanDebugUtils::operator=(VulkanDebugUtils &&o) = default;
VulkanDebugUtils::~VulkanDebugUtils()                               = default;