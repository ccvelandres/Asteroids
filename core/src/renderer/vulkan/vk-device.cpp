#include <renderer/vulkan/vk-device.hpp>
#include <renderer/vulkan/vk-utils.hpp>

#include <utils/logging.hpp>

struct VulkanDevice::QueueConfig
{
    uint32_t graphicsQueueIndex;
    uint32_t presentQueueIndex;
};

VulkanDevice::QueueConfig VulkanDevice::getQueueConfig(const vk::PhysicalDevice &physicalDevice,
                                                       const vk::SurfaceKHR &surface)
{
    L_TAG("VulkanDevice::getQueueConfig");

    constexpr uint32_t maxIndex = std::numeric_limits<uint32_t>::max();
    uint32_t graphicsQueueIndex = maxIndex;
    uint32_t presentQueueIndex = maxIndex;

    std::vector<vk::QueueFamilyProperties> queueFamilyProperties =
        physicalDevice.getQueueFamilyProperties();

    for (uint32_t i = 0; i < queueFamilyProperties.size(); i++)
    {
        vk::QueueFamilyProperties &queueProperties = queueFamilyProperties[i];

        if (queueProperties.queueCount > 0 && queueProperties.queueFlags & vk::QueueFlagBits::eGraphics)
        {
            if (graphicsQueueIndex == maxIndex)
                graphicsQueueIndex = i;

            if (physicalDevice.getSurfaceSupportKHR(i, surface))
            {
                graphicsQueueIndex = presentQueueIndex = i;
                L_DEBUG("Using same queue for graphics and presentation");
                break;
            }
        }
    }

    // Check if we have graphic queue
    if (graphicsQueueIndex == maxIndex)
    {
        L_THROW_RUNTIME("Could not find a graphics queue");
    }

    // If we have no presentation queue, look for one
    if (presentQueueIndex == maxIndex)
    {
        for (uint32_t i = 0; i < queueFamilyProperties.size(); i++)
        {
            if (physicalDevice.getSurfaceSupportKHR(i, surface))
            {
                presentQueueIndex = i;
                break;
            }
        }
    }

    // Check if we have presentation queue
    if (presentQueueIndex == maxIndex)
    {
        L_THROW_RUNTIME("Could not find a presentation queue");
    }

    return QueueConfig{graphicsQueueIndex, presentQueueIndex};
}

VulkanDevice::VulkanDevice(SDL_Window *window,
                           const vk::Instance &instance,
                           const vk::PhysicalDevice &physicalDevice,
                           const vk::SurfaceKHR &surface)
{
    L_TAG("VulkanDevice::VulkanDevice");

    const float queuePriority = 1.0f;

    QueueConfig queueConfig = getQueueConfig(physicalDevice, surface);

    // Queue config for graphics queue
    std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos = {
        vk::DeviceQueueCreateInfo(
            vk::DeviceQueueCreateFlags(),
            queueConfig.graphicsQueueIndex,
            1,
            &queuePriority)};

    // Queue config for presentation queue
    if (queueConfig.graphicsQueueIndex != queueConfig.presentQueueIndex)
    {
        queueCreateInfos.push_back(
            vk::DeviceQueueCreateInfo(
                vk::DeviceQueueCreateFlags(),
                queueConfig.presentQueueIndex,
                1,
                &queuePriority));
    }

    std::vector<const char *> deviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME};

    vk::DeviceCreateInfo deviceCreateInfo = vk::DeviceCreateInfo(
        vk::DeviceCreateFlags(),
        static_cast<uint32_t>(queueCreateInfos.size()),
        queueCreateInfos.data(),
        0,
        nullptr,
        static_cast<uint32_t>(deviceExtensions.size()),
        deviceExtensions.data(),
        nullptr);

    // Create the device
    m_device = physicalDevice.createDeviceUnique(deviceCreateInfo);

    // Get the queues
    m_graphicsQueue = m_device->getQueue(queueConfig.graphicsQueueIndex, 0);
    m_presentQueue = m_device->getQueue(queueConfig.presentQueueIndex, 0);

    L_DEBUG("VulkanDevice successfully created");
}

VulkanDevice::~VulkanDevice()
{
}

vk::Device &VulkanDevice::getDevice()
{
    return *m_device;
}

vk::Queue &VulkanDevice::getGraphicsQueue()
{
    return m_graphicsQueue;
}

vk::Queue &VulkanDevice::getPresentQueue()
{
    return m_presentQueue;
}