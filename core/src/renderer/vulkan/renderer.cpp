#include <renderer.hpp>
#include <utils/logging.hpp>

#include <exception>
#include <algorithm>

// const vk::ApplicationInfo applicationInfo{
    // "ApplicationName",
    // VK_MAKE_VERSION(0, 1, 0),
    // "NoEngine",
    // VK_MAKE_VERSION(0, 1, 0),
    // VK_MAKE_VERSION(0, 1, 0)};
const VkApplicationInfo applicationInfo{
    .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
    .pNext = NULL,
    .pApplicationName = "No Engine",
    .applicationVersion = VK_MAKE_VERSION(0, 1, 0),
    .pEngineName = "NoE",
    .engineVersion = 0,
    .apiVersion = VK_API_VERSION_1_0,
};

const std::vector<std::string> validationLayers = {
    "VK_LAYER_LUNARG_standard_validation",
    "VK_LAYER_KHRONOS_validation"};

std::vector<std::string> desiredValidationLayers;
std::vector<const char *> desiredValidationLayersChar;
std::vector<const char *> extensionNames;

VulkanRenderer::VulkanRenderer(SDL_Window *window) : m_window(window)
{
    L_TAG("VulkanRenderer::VulkanRenderer");

    /** Create Vulkan Instance */
    {
        /** Check if validation layers are available and create a desired layers */
        desiredValidationLayers.clear();
        for (const auto &properties : vk::enumerateInstanceLayerProperties())
        {
            L_DEBUG("Available Instance Layers: {}", properties.layerName);
            if (std::find(validationLayers.begin(),
                          validationLayers.end(),
                          properties.layerName) != validationLayers.end())
            {
                L_DEBUG("\tAdding to desired layer");
                desiredValidationLayers.push_back(properties.layerName);
            }
        }
        desiredValidationLayersChar.clear();
        for (const auto &layer : desiredValidationLayers)
            desiredValidationLayersChar.push_back(layer.c_str());

        /** Get Instance extensions */
        unsigned int extensionCount = 0;
        extensionNames.clear();
        SDL_Vulkan_GetInstanceExtensions(m_window, &extensionCount, nullptr);
        extensionNames.reserve(extensionCount);
        SDL_Vulkan_GetInstanceExtensions(m_window, &extensionCount, extensionNames.data());

        extensionNames.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

        // vk::InstanceCreateInfo instanceCreateInfo{
        //     vk::InstanceCreateFlags(),
        //     &applicationInfo,
        //     static_cast<uint32_t>(desiredValidationLayersChar.size()),
        //     desiredValidationLayersChar.data(),
        //     static_cast<uint32_t>(extensionCount),
        //     extensionNames.data()};
        VkInstanceCreateInfo instanceCreateInfo = {};
        instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        instanceCreateInfo.pApplicationInfo = &applicationInfo;
        instanceCreateInfo.enabledLayerCount = 0; //desiredValidationLayersChar.size();
        instanceCreateInfo.ppEnabledLayerNames = nullptr; //desiredValidationLayersChar.data();
        instanceCreateInfo.enabledExtensionCount = 0; //extensionCount;
        instanceCreateInfo.ppEnabledExtensionNames = nullptr; //extensionNames.data();

        /** Create Vulkan Instance */
        try
        {
            // m_instance = vk::createInstance(instanceCreateInfo);
            VkInstance instance;
            vkCreateInstance(&instanceCreateInfo, nullptr, &instance);
        }
        catch (const std::exception &err)
        {
            std::cerr << err.what() << std::endl;
        }
    }

    /** Create/Select Physical Device */
    {
        std::vector<vk::PhysicalDevice> physicalDevices =
            m_instance.enumeratePhysicalDevices();

        if (physicalDevices.empty())
        {
            L_ERROR("No VulkanPhysicalDevices found");
            throw std::runtime_error("No VulkanPhysicalDevices found");
        }

        m_physicalDevice = physicalDevices[0];

        // Look if there is a discrete GPU, if not, stick with the first device
        for (auto &physicalDevice : physicalDevices)
        {
            vk::PhysicalDeviceProperties deviceProperties =
                physicalDevice.getProperties();

            if (deviceProperties.deviceType != vk::PhysicalDeviceType::eDiscreteGpu)
            {
                m_physicalDevice = physicalDevice;
                break;
            }
        }

        // Store device properties
        m_physicalDeviceProperties = m_physicalDevice.getProperties();

        L_INFO("Physical Device: {}", m_physicalDeviceProperties.deviceName);
        L_INFO("\tSwapchain: {}", checkSwapchainSupport(m_physicalDevice));

        // std::vector<VkPhysicalDevice> physicalDevices;
        // uint32_t physicalDeviceCount = 0;

        // vkEnumeratePhysicalDevices(m_instance, &physicalDeviceCount, nullptr);
        // physicalDevices.resize(physicalDeviceCount);
        // vkEnumeratePhysicalDevices(m_instance, &physicalDeviceCount, physicalDevices.data());

        // Select first device
        /** @todo maybe pick dGPU */
        // m_physicalDevice = physicalDevices[0];
    }

    /** Create KHR Surface */
    {
        m_surface = createSurface(m_window, m_instance);
    }

    /** Select VK QueueFamily */
    {
        constexpr uint32_t maxIndex = std::numeric_limits<uint32_t>::max();
        uint32_t graphicsIndex = maxIndex;
        uint32_t presentIndex = maxIndex;

        std::vector<vk::QueueFamilyProperties> queueFamilyProperties =
            m_physicalDevice.getQueueFamilyProperties();

        for (uint32_t i = 0; i < queueFamilyProperties.size(); i++)
        {
            vk::QueueFamilyProperties &queueProperties = queueFamilyProperties[i];

            if (queueProperties.queueCount > 0 && queueProperties.queueFlags & vk::QueueFlagBits::eGraphics)
            {
                if (graphicsIndex == maxIndex)
                    graphicsIndex = i;

                if (m_physicalDevice.getSurfaceSupportKHR(i, m_surface))
                {
                    graphicsIndex = presentIndex = i;
                    L_DEBUG("Using same queue for graphics and presentation");
                    break;
                }
            }
        }

        // Check if we have graphic queue
        if (graphicsIndex == maxIndex)
        {
            L_ERROR("Could not find a graphics queue");
            throw std::runtime_error("Could not find a graphics queue");
        }

        // If we have no presentation queue, look for one
        if (presentIndex == maxIndex)
        {
            for (uint32_t i = 0; i < queueFamilyProperties.size(); i++)
            {
                if (m_physicalDevice.getSurfaceSupportKHR(i, m_surface))
                {
                    presentIndex = i;
                    break;
                }
            }
        }

        // Check if we have presentation queue
        if (presentIndex == maxIndex)
        {
            L_ERROR("Could not find a presentation queue");
            throw std::runtime_error("Could not find a presentation queue");
        }

        m_graphicsQueueIndex = graphicsIndex;
        m_presentQueueIndex = presentIndex;

        // std::vector<VkQueueFamilyProperties> queueFamilyProperties;
        // uint32_t queueFamilyCount;

        // vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &queueFamilyCount, nullptr);
        // queueFamilyProperties.resize(queueFamilyCount);
        // vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &queueFamilyCount, queueFamilyProperties.data());

        // int graphicsIndex = -1;
        // int presentIndex = -1;

        // for (int i = 0; i < queueFamilyProperties.size(); i++)
        // {
        //     if (queueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
        //         graphicsIndex = i;

        //     VkBool32 presentSupport = false;
        //     vkGetPhysicalDeviceSurfaceSupportKHR(m_physicalDevice, i, m_surface, &presentSupport);
        //     if (queueFamilyProperties[i].queueCount > 0 && presentSupport)
        //     {
        //         presentIndex = i;
        //     }

        //     if (graphicsIndex != -1)
        //         break;
        // }

        // m_graphicsQueueIndex = graphicsIndex;
        // m_presentQueueIndex = presentIndex;
    }

    /** Create VK Queue */
    {
        constexpr float queuePriority = 1.0f;
        const std::vector<const char *> deviceExtensions = {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME};

        // Queue config for graphics queue
        std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos = {
            vk::DeviceQueueCreateInfo(
                vk::DeviceQueueCreateFlags(),
                m_graphicsQueueIndex,
                1,
                &queuePriority)};

        // Queue config for presentation queue
        if (m_graphicsQueueIndex != m_presentQueueIndex)
        {
            queueCreateInfos.push_back(
                vk::DeviceQueueCreateInfo(
                    vk::DeviceQueueCreateFlags(),
                    m_presentQueueIndex,
                    1,
                    &queuePriority));
        }

        vk::DeviceCreateInfo deviceCreateInfo = vk::DeviceCreateInfo(
            vk::DeviceCreateFlags(),
            queueCreateInfos.size(),
            queueCreateInfos.data(),
            0,
            nullptr,
            deviceExtensions.size(),
            deviceExtensions.data(),
            nullptr);

        // Create the device
        m_device = m_physicalDevice.createDevice(deviceCreateInfo);

        // Get the queues
        m_graphicsQueue = m_device.getQueue(m_graphicsQueueIndex, 0);
        m_presentQueue = m_device.getQueue(m_presentQueueIndex, 0);

        // const std::vector<const char *> deviceExtensions = {
        //     VK_KHR_SWAPCHAIN_EXTENSION_NAME};

        // float queuePriorities[1] = {0.0};
        // std::vector<uint32_t> uniqueQueueFamily = {m_graphicsQueueIndex, m_presentQueueIndex};
        // std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        // for (const auto &queueFamily : uniqueQueueFamily)
        // {
        //     VkDeviceQueueCreateInfo queueCreateInfo = {};
        //     queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
        //     queueCreateInfo.pNext = nullptr,
        //     queueCreateInfo.queueFamilyIndex = queueFamily,
        //     queueCreateInfo.queueCount = 1,
        //     queueCreateInfo.pQueuePriorities = &queuePriorities[0];
        //     queueCreateInfos.push_back(queueCreateInfo);
        // }

        // VkPhysicalDeviceFeatures deviceFeatures = {};
        // deviceFeatures.samplerAnisotropy = VK_TRUE;

        // VkDeviceCreateInfo deviceCreateInfo = {};
        // deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        // deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();
        // deviceCreateInfo.queueCreateInfoCount = queueCreateInfos.size();
        // deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions.data();
        // deviceCreateInfo.enabledExtensionCount = deviceExtensions.size();
        // deviceCreateInfo.pEnabledFeatures = &deviceFeatures;
        // deviceCreateInfo.ppEnabledLayerNames = validationLayers.data();
        // deviceCreateInfo.enabledLayerCount = validationLayers.size();

        // vkCreateDevice(m_physicalDevice, &deviceCreateInfo, nullptr, &m_device);

        // vkGetDeviceQueue(m_device, m_graphicsQueueIndex, 0, &m_graphicsQueue);
        // vkGetDeviceQueue(m_device, m_presentQueueIndex, 0, &m_presentQueue);
    }
}

VulkanRenderer::~VulkanRenderer()
{
    vkDestroyDevice(m_device, nullptr);
    vkDestroyInstance(m_instance, nullptr);
}

vk::SurfaceKHR VulkanRenderer::createSurface(SDL_Window *window, vk::Instance &instance)
{
    L_TAG("VulkanRenderer::createSurface");

    VkSurfaceKHR surface;
    if (SDL_Vulkan_CreateSurface(window, instance, &surface))
    {
        L_ERROR("Could not create a Vulkan surface");
        throw std::runtime_error("Could not create a Vulkan surface");
    }
    return vk::SurfaceKHR(surface);
}

bool VulkanRenderer::checkSwapchainSupport(vk::PhysicalDevice &device)
{
    // Check if device supports swapchain
    bool swapchainSupport = false;
    for (const auto &extension : device.enumerateDeviceExtensionProperties())
    {
        if (extension.extensionName == VK_KHR_SWAPCHAIN_EXTENSION_NAME)
        {
            swapchainSupport = true;
            break;
        }
    }

    return swapchainSupport;
}