#include <core/graphics/renderer/vulkan/vk-device.hpp>
#include <core/graphics/renderer/vulkan/vk-utils.hpp>

#include <utils/logging.hpp>

struct QueueConfig
{
    uint32_t graphicsQueueIndex;
    uint32_t presentQueueIndex;
};

QueueConfig getQueueConfig(const vk::PhysicalDevice &physicalDevice, const vk::SurfaceKHR &surface)
{
    L_TAG("getQueueConfig");

    constexpr uint32_t maxIndex           = std::numeric_limits<uint32_t>::max();
    uint32_t           graphicsQueueIndex = maxIndex;
    uint32_t           presentQueueIndex  = maxIndex;

    std::vector<vk::QueueFamilyProperties> queueFamilyProperties = physicalDevice.getQueueFamilyProperties();

    for (uint32_t i = 0; i < queueFamilyProperties.size(); i++)
    {
        vk::QueueFamilyProperties &queueProperties = queueFamilyProperties[i];

        if (queueProperties.queueCount > 0 && queueProperties.queueFlags & vk::QueueFlagBits::eGraphics)
        {
            if (graphicsQueueIndex == maxIndex) graphicsQueueIndex = i;

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

vk::UniqueDevice createDevice(const VulkanPhysicalDevice &physicalDevice, const QueueConfig &queueConfig)
{
    L_TAG("createDevice");

    const float queuePriority = 1.0f;

    // Queue config for graphics queue
    std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos = {
        vk::DeviceQueueCreateInfo(vk::DeviceQueueCreateFlags(), queueConfig.graphicsQueueIndex, 1, &queuePriority)};

    // Queue config for presentation queue
    if (queueConfig.graphicsQueueIndex != queueConfig.presentQueueIndex)
    {
        queueCreateInfos.push_back(
            vk::DeviceQueueCreateInfo(vk::DeviceQueueCreateFlags(), queueConfig.presentQueueIndex, 1, &queuePriority));
    }

    /** Physical Device features */
    vk::PhysicalDeviceFeatures physicalDeviceFeatures;

    if (physicalDevice.getShaderMultisamplingSupport()) physicalDeviceFeatures.sampleRateShading = true;

    std::vector<const char *> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

    vk::DeviceCreateInfo deviceCreateInfo(vk::DeviceCreateFlags(),
                                          static_cast<uint32_t>(queueCreateInfos.size()),
                                          queueCreateInfos.data(),
                                          0,
                                          nullptr,
                                          static_cast<uint32_t>(deviceExtensions.size()),
                                          deviceExtensions.data(),
                                          &physicalDeviceFeatures);

    // Create the device
    return physicalDevice.getPhysicalDevice().createDeviceUnique(deviceCreateInfo);
}

std::vector<vk::UniqueSemaphore> createSemaphores(const vk::Device &device, const uint32_t count)
{
    L_TAG("createSemaphores");
    std::vector<vk::UniqueSemaphore> semaphores;
    vk::SemaphoreCreateInfo          semaphoreCreateInfo;

    for (int i = 0; i < count; i++)
    {
        semaphores.push_back(device.createSemaphoreUnique(semaphoreCreateInfo));
    }

    L_TRACE("Created {} semaphores", count);
    return semaphores;
}

std::vector<vk::UniqueFence> createFences(const vk::Device &device, const uint32_t count)
{
    L_TAG("createFences");
    std::vector<vk::UniqueFence> fences;

    vk::FenceCreateInfo fenceCreateinfo(vk::FenceCreateFlagBits::eSignaled);

    for (int i = 0; i < count; i++)
    {
        fences.push_back(device.createFenceUnique(fenceCreateinfo));
    }
    L_TRACE("Created {} fences", count);
    return fences;
}

struct VulkanDevice::Internal
{
    const QueueConfig      queueConfig;
    const vk::UniqueDevice device;
    const vk::Queue        graphicsQueue;
    const vk::Queue        presentQueue;

    Internal(SDL_Window *window, const VulkanPhysicalDevice &physicalDevice, const vk::SurfaceKHR &surface)
        : queueConfig(::getQueueConfig(physicalDevice.getPhysicalDevice(), surface)),
          device(::createDevice(physicalDevice, queueConfig)),
          graphicsQueue(device->getQueue(queueConfig.graphicsQueueIndex, 0)),
          presentQueue(device->getQueue(queueConfig.presentQueueIndex, 0))
    {
        L_TAG("VulkanDevice::Internal");
        L_TRACE("Internal resources initialized ({})", static_cast<void *>(this));
    }

    ~Internal()
    {
        L_TAG("VulkanDevice::~Internal");
        L_TRACE("Internal resources freed ({})", static_cast<void *>(this));
    }
};

VulkanDevice::VulkanDevice(SDL_Window *window, const VulkanPhysicalDevice &physicalDevice, const VulkanSurface &surface)
    : m_internal(std::make_unique<Internal>(window, physicalDevice, surface.getSurface()))
{
}

VulkanDevice::VulkanDevice(VulkanDevice &&o)            = default;
VulkanDevice &VulkanDevice::operator=(VulkanDevice &&o) = default;
VulkanDevice::~VulkanDevice()                           = default;

const vk::Device &VulkanDevice::getDevice() const { return *m_internal->device; }

const vk::Queue &VulkanDevice::getGraphicsQueue() const { return m_internal->graphicsQueue; }

const vk::Queue &VulkanDevice::getPresentQueue() const { return m_internal->presentQueue; }

const uint32_t VulkanDevice::getGraphicsQueueIndex() const { return m_internal->queueConfig.graphicsQueueIndex; }

const uint32_t VulkanDevice::getPresentQueueIndex() const { return m_internal->queueConfig.presentQueueIndex; }

const bool VulkanDevice::hasDiscreteQueue() const
{
    return m_internal->queueConfig.graphicsQueueIndex != m_internal->queueConfig.presentQueueIndex;
}

std::vector<vk::UniqueSemaphore> VulkanDevice::createSemaphores(const uint32_t count) const
{
    return ::createSemaphores(*m_internal->device, count);
}

std::vector<vk::UniqueFence> VulkanDevice::createFences(const uint32_t count) const
{
    return ::createFences(*m_internal->device, count);
}