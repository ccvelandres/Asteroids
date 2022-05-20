#include <renderer/vulkan/vk-physicalDevice.hpp>
#include <renderer/vulkan/vk-utils.hpp>

#include <utils/logging.hpp>

VulkanPhysicalDevice::VulkanPhysicalDevice(VulkanInstance &instance)
{
    L_TAG("VulkanPhysicalDevice::VulkanPhysicalDevice");

    std::vector<vk::PhysicalDevice> physicalDevices =
        instance.getInstance().enumeratePhysicalDevices();

    if (physicalDevices.empty())
    {
        L_THROW_RUNTIME("No VulkanPhysicalDevices found");
    }

    m_physicalDevice = physicalDevices[0];

    // Look if there is a discrete GPU, if not, stick with the first device
    for (auto &physicalDevice : physicalDevices)
    {
        vk::PhysicalDeviceProperties deviceProperties =
            physicalDevice.getProperties();

        L_DEBUG("Found PhysicalDevice: {}", deviceProperties.deviceName);

        if (deviceProperties.deviceType == vk::PhysicalDeviceType::eDiscreteGpu)
        {
            m_physicalDevice = physicalDevice;
        }
    }

    // Store device properties
    vk::PhysicalDeviceProperties m_physicalDeviceProperties = m_physicalDevice.getProperties();

    L_INFO("Selected PhysicalDevice: {}", m_physicalDeviceProperties.deviceName);
    if (VulkanUtils::checkSwapchainSupport(m_physicalDevice))
        L_INFO("\tSwapchain support found");
    else
        L_THROW_RUNTIME("Device does not support swapchain");
}

VulkanPhysicalDevice::~VulkanPhysicalDevice()
{
}

vk::PhysicalDevice &VulkanPhysicalDevice::getPhysicalDevice()
{
    return m_physicalDevice;
}