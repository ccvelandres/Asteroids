#include <core/graphics/renderer/vulkan/vk-physicalDevice.hpp>
#include <core/graphics/renderer/vulkan/vk-utils.hpp>

#include <utils/logging.hpp>

vk::PhysicalDevice createPhysicalDevice(const vk::Instance &instance)
{
    L_TAG("createPhysicalDevice");

    vk::PhysicalDevice physicalDevice;

    std::vector<vk::PhysicalDevice> physicalDevices = instance.enumeratePhysicalDevices();

    if (physicalDevices.empty())
    {
        L_THROW_RUNTIME("No VulkanPhysicalDevices found");
    }

    physicalDevice = physicalDevices[0];

    // Look if there is a discrete GPU, if not, stick with the first device
    for (auto &physicalDevice : physicalDevices)
    {
        vk::PhysicalDeviceProperties deviceProperties = physicalDevice.getProperties();

        L_DEBUG("Found PhysicalDevice: {}", deviceProperties.deviceName);

        if (deviceProperties.deviceType == vk::PhysicalDeviceType::eDiscreteGpu)
        {
            physicalDevice = physicalDevice;
        }
    }

    // Store device properties
    vk::PhysicalDeviceProperties physicalDeviceProperties = physicalDevice.getProperties();

    L_INFO("Selected PhysicalDevice: {}", physicalDeviceProperties.deviceName);
    if (VulkanUtils::checkSwapchainSupport(physicalDevice))
        L_INFO("\tSwapchain support found");
    else
        L_THROW_RUNTIME("Device does not support swapchain");

    return physicalDevice;
}

vk::SampleCountFlagBits getMultisampling(const vk::PhysicalDevice &physicalDevice)
{
    L_TAG("getMultisampling");

    vk::PhysicalDeviceProperties deviceProperties   = physicalDevice.getProperties();
    vk::SampleCountFlags         deviceSampleCounts = deviceProperties.limits.framebufferColorSampleCounts;
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

vk::Format getDepthFormat(const vk::PhysicalDevice &physicalDevice)
{
    L_TAG("getDepthFormat");

    vk::FormatProperties formatProperties = physicalDevice.getFormatProperties(vk::Format::eD32Sfloat);

    if (formatProperties.optimalTilingFeatures & vk::FormatFeatureFlagBits::eDepthStencilAttachment)
    {
        return vk::Format::eD32Sfloat;
    }

    L_THROW_RUNTIME("32bit signed depth stencil format unsupported");
}

bool getShaderMultisamplingSupport(const vk::PhysicalDevice &physicalDevice)
{
    return physicalDevice.getFeatures().sampleRateShading;
}

struct VulkanPhysicalDevice::Internal
{
    const vk::PhysicalDevice      physicalDevice;
    const vk::SampleCountFlagBits multisampling;
    const vk::Format              depthFormat;
    const bool                    shaderMultisamplingSupport;

    Internal(const vk::Instance &instance)
        : physicalDevice(::createPhysicalDevice(instance)),
          multisampling(::getMultisampling(physicalDevice)),
          depthFormat(::getDepthFormat(physicalDevice)),
          shaderMultisamplingSupport(::getShaderMultisamplingSupport(physicalDevice))
    {
        L_TAG("VulkanPhysicalDevice::Internal");
        L_TRACE("Internal resources initialized ({})", static_cast<void *>(this));
    }

    ~Internal()
    {
        L_TAG("VulkanPhysicalDevice::~Internal");
        L_TRACE("Internal resources freed ({})", static_cast<void *>(this));
    }
};

VulkanPhysicalDevice::VulkanPhysicalDevice(const VulkanInstance &instance)
    : m_internal(std::make_unique<Internal>(instance.getInstance()))
{
}

VulkanPhysicalDevice::VulkanPhysicalDevice(VulkanPhysicalDevice &&o)            = default;
VulkanPhysicalDevice &VulkanPhysicalDevice::operator=(VulkanPhysicalDevice &&o) = default;
VulkanPhysicalDevice::~VulkanPhysicalDevice()                                   = default;

const vk::PhysicalDevice &VulkanPhysicalDevice::getPhysicalDevice() const { return m_internal->physicalDevice; }

const vk::SampleCountFlagBits VulkanPhysicalDevice::getMultisampling() const { return m_internal->multisampling; }

const vk::Format VulkanPhysicalDevice::getDepthFormat() const { return m_internal->depthFormat; }

const bool VulkanPhysicalDevice::getShaderMultisamplingSupport() const
{
    return m_internal->shaderMultisamplingSupport;
}

uint32_t VulkanPhysicalDevice::getMemoryTypeIndex(const uint32_t filter, const vk::MemoryPropertyFlags &flags) const
{
    L_TAG("VulkanPhysicalDevice::getMemoryTypeIndex");

    vk::PhysicalDeviceMemoryProperties memoryProperties = m_internal->physicalDevice.getMemoryProperties();

    for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++)
    {
        if (filter & (1 << i) && (memoryProperties.memoryTypes[i].propertyFlags & flags) == flags)
        {
            return i;
        }
    }

    L_THROW_RUNTIME("Failed to find suitable memory type");
}