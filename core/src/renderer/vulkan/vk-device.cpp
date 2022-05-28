#include <renderer/vulkan/vk-device.hpp>
#include <renderer/vulkan/vk-utils.hpp>

#include <utils/logging.hpp>

struct QueueConfig
{
    uint32_t graphicsQueueIndex;
    uint32_t presentQueueIndex;
};

QueueConfig getQueueConfig( const vk::PhysicalDevice &physicalDevice,
                            const vk::SurfaceKHR     &surface )
{
    L_TAG( "getQueueConfig" );

    constexpr uint32_t maxIndex = std::numeric_limits<uint32_t>::max();
    uint32_t           graphicsQueueIndex = maxIndex;
    uint32_t           presentQueueIndex  = maxIndex;

    std::vector<vk::QueueFamilyProperties> queueFamilyProperties =
        physicalDevice.getQueueFamilyProperties();

    for ( uint32_t i = 0; i < queueFamilyProperties.size(); i++ )
    {
        vk::QueueFamilyProperties &queueProperties = queueFamilyProperties [i];

        if ( queueProperties.queueCount > 0
             && queueProperties.queueFlags & vk::QueueFlagBits::eGraphics )
        {
            if ( graphicsQueueIndex == maxIndex ) graphicsQueueIndex = i;

            if ( physicalDevice.getSurfaceSupportKHR( i, surface ) )
            {
                graphicsQueueIndex = presentQueueIndex = i;
                L_DEBUG( "Using same queue for graphics and presentation" );
                break;
            }
        }
    }

    // Check if we have graphic queue
    if ( graphicsQueueIndex == maxIndex )
    {
        L_THROW_RUNTIME( "Could not find a graphics queue" );
    }

    // If we have no presentation queue, look for one
    if ( presentQueueIndex == maxIndex )
    {
        for ( uint32_t i = 0; i < queueFamilyProperties.size(); i++ )
        {
            if ( physicalDevice.getSurfaceSupportKHR( i, surface ) )
            {
                presentQueueIndex = i;
                break;
            }
        }
    }

    // Check if we have presentation queue
    if ( presentQueueIndex == maxIndex )
    {
        L_THROW_RUNTIME( "Could not find a presentation queue" );
    }

    return QueueConfig{ graphicsQueueIndex, presentQueueIndex };
}

vk::UniqueDevice createDevice( const vk::PhysicalDevice &physicalDevice,
                               const QueueConfig        &queueConfig )
{
    L_TAG( "createDevice" );

    const float queuePriority = 1.0f;

    // Queue config for graphics queue
    std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos = {
        vk::DeviceQueueCreateInfo( vk::DeviceQueueCreateFlags(),
                                   queueConfig.graphicsQueueIndex,
                                   1,
                                   &queuePriority ) };

    // Queue config for presentation queue
    if ( queueConfig.graphicsQueueIndex != queueConfig.presentQueueIndex )
    {
        queueCreateInfos.push_back(
            vk::DeviceQueueCreateInfo( vk::DeviceQueueCreateFlags(),
                                       queueConfig.presentQueueIndex,
                                       1,
                                       &queuePriority ) );
    }

    std::vector<const char *> deviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME };

    vk::DeviceCreateInfo deviceCreateInfo =
        vk::DeviceCreateInfo( vk::DeviceCreateFlags(),
                              static_cast<uint32_t>( queueCreateInfos.size() ),
                              queueCreateInfos.data(),
                              0,
                              nullptr,
                              static_cast<uint32_t>( deviceExtensions.size() ),
                              deviceExtensions.data(),
                              nullptr );

    // Create the device
    return physicalDevice.createDeviceUnique( deviceCreateInfo );
}

struct VulkanDevice::Internal
{
    const QueueConfig      queueConfig;
    const vk::UniqueDevice device;
    const vk::Queue        graphicsQueue;
    const vk::Queue        presentQueue;

    Internal( SDL_Window               *window,
              const vk::PhysicalDevice &physicalDevice,
              const vk::SurfaceKHR     &surface )
        : queueConfig( ::getQueueConfig( physicalDevice, surface ) ),
          device( ::createDevice( physicalDevice, queueConfig ) ),
          graphicsQueue( device->getQueue( queueConfig.graphicsQueueIndex, 0 ) ),
          presentQueue( device->getQueue( queueConfig.presentQueueIndex, 0 ) )
    {
        L_TAG( "VulkanDevice::Internal" );
        L_DEBUG( "Device successfully created" );
    }
};

VulkanDevice::VulkanDevice( SDL_Window                 *window,
                            const VulkanPhysicalDevice &physicalDevice,
                            const VulkanSurface        &surface )
    : m_internal( std::make_unique<Internal>( window,
                                              physicalDevice.getPhysicalDevice(),
                                              surface.getSurface() ) )
{
}

VulkanDevice::~VulkanDevice() {}

const vk::Device &VulkanDevice::getDevice() const
{
    return *m_internal->device;
}

const vk::Queue &VulkanDevice::getGraphicsQueue() const
{
    return m_internal->graphicsQueue;
}

const vk::Queue &VulkanDevice::getPresentQueue() const
{
    return m_internal->presentQueue;
}

const uint32_t VulkanDevice::getGraphicsQueueIndex() const
{
    return m_internal->queueConfig.graphicsQueueIndex;
}

const uint32_t VulkanDevice::getPresentQueueIndex() const
{
    return m_internal->queueConfig.presentQueueIndex;
}

const bool VulkanDevice::hasDiscreteQueue() const
{
    return m_internal->queueConfig.graphicsQueueIndex
        != m_internal->queueConfig.presentQueueIndex;
}