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

vk::SampleCountFlagBits VulkanPhysicalDevice::getMultisampling()
{
    L_TAG("VulkanPhysicalDevice::getMultisampling");

    vk::PhysicalDeviceProperties deviceProperties = m_physicalDevice.getProperties();
    vk::SampleCountFlags deviceSampleCounts = deviceProperties.limits.framebufferColorSampleCounts;
    const std::vector<vk::SampleCountFlagBits> preferredSampleCounts = {vk::SampleCountFlagBits::e8,
                                                                        vk::SampleCountFlagBits::e4,
                                                                        vk::SampleCountFlagBits::e2,
                                                                        vk::SampleCountFlagBits::e1};

    for (const auto &sampleCount : preferredSampleCounts)
    {
        if (deviceSampleCounts & sampleCount)
        {
            return sampleCount;
        }
    }

    L_THROW_RUNTIME("Multisampling not supported");
}

vk::Format VulkanPhysicalDevice::getDepthFormat()
{
    L_TAG("VulkanPhysicalDevice::getDepthFormat");

    vk::FormatProperties formatProperties = m_physicalDevice.getFormatProperties(vk::Format::eD32Sfloat);

    if (formatProperties.optimalTilingFeatures & vk::FormatFeatureFlagBits::eDepthStencilAttachment)
    {
        return vk::Format::eD32Sfloat;
    }

    L_THROW_RUNTIME("32bit signed depth stencil format unsupported");
}