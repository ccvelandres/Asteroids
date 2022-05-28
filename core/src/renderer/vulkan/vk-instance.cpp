#include <renderer/vulkan/vk-config.hpp>
#include <renderer/vulkan/vk-instance.hpp>
#include <renderer/vulkan/vk-utils.hpp>

#include <utils/logging.hpp>

std::vector<const char *> getValidationLayers()
{
    L_TAG( "VulkanInstance::getValidationLayers" );

    std::vector<const char *>        desiredValidationLayers;
    std::vector<vk::LayerProperties> availableLayers = vk::enumerateInstanceLayerProperties();
    for ( const auto &layer : VulkanConfig::getValidationLayers() )
    {
        if ( VulkanUtils::checkValidationLayerSupport( layer, availableLayers ) )
        {
            desiredValidationLayers.push_back( layer );
            L_DEBUG( "ValidationLayer requested: {}", layer );
        }
        else
        {
            L_WARN( "ValidationLayer not available: {}", layer );
        }
    }

    return desiredValidationLayers;
}

std::vector<const char *> getRequiredExtensions( SDL_Window *window )
{
    L_TAG( "VulkanInstance::getRequiredExtensions" );

    std::vector<const char *> extensionNames;
    {
        unsigned int              extensionCount = 0;
        std::vector<const char *> sdlExtensions;
        SDL_Vulkan_GetInstanceExtensions( window, &extensionCount, nullptr );
        sdlExtensions.reserve( extensionCount );
        SDL_Vulkan_GetInstanceExtensions( window, &extensionCount, sdlExtensions.data() );
        for ( int i = 0; i < extensionCount; i++ )
        {
            L_DEBUG( "Adding SDLRequiredInstanceExtension: {}", sdlExtensions [i] );
            extensionNames.push_back( sdlExtensions [i] );
        }
    }

    for ( const auto &extension : VulkanConfig::getInstanceExtensions() )
    {
        extensionNames.push_back( extension );
    }

    return extensionNames;
}

static vk::UniqueInstance createInstance( SDL_Window *window )
{
    L_TAG( "VulkanInstance::createInstance" );

    /** Query some stuff about vulkan instance */
    uint32_t version = vk::enumerateInstanceVersion();
    L_DEBUG( "Vulkan API Version: {}.{}.{}",
             VK_VERSION_MAJOR( version ),
             VK_VERSION_MINOR( version ),
             VK_VERSION_PATCH( version ) );

    /** Check if validation layers are available and create a desired layers */
    std::vector<const char *> desiredValidationLayers = getValidationLayers();

    /** Get Instance extensions */
    std::vector<const char *> extensionNames = getRequiredExtensions( window );

    vk::InstanceCreateInfo instanceCreateInfo{ vk::InstanceCreateFlags(),
                                               VulkanConfig::getApplicationInfo(),
                                               static_cast<uint32_t>( desiredValidationLayers.size() ),
                                               desiredValidationLayers.data(),
                                               static_cast<uint32_t>( extensionNames.size() ),
                                               extensionNames.data() };
}

struct VulkanInstance::internal
{
    const vk::UniqueInstance instance;

    internal() : instance() {}
};

VulkanInstance::VulkanInstance( SDL_Window *window )
{
    L_TAG( "VulkanInstance::VulkanInstance" );

    /** Query some stuff about vulkan instance */
    uint32_t version = vk::enumerateInstanceVersion();
    L_DEBUG( "Vulkan API Version: {}.{}.{}",
             VK_VERSION_MAJOR( version ),
             VK_VERSION_MINOR( version ),
             VK_VERSION_PATCH( version ) );

    /** Check if validation layers are available and create a desired layers */
    std::vector<const char *> desiredValidationLayers = getValidationLayers();

    /** Get Instance extensions */
    std::vector<const char *> extensionNames = getRequiredExtensions( window );

    vk::InstanceCreateInfo instanceCreateInfo{ vk::InstanceCreateFlags(),
                                               VulkanConfig::getApplicationInfo(),
                                               static_cast<uint32_t>( desiredValidationLayers.size() ),
                                               desiredValidationLayers.data(),
                                               static_cast<uint32_t>( extensionNames.size() ),
                                               extensionNames.data() };

    m_instance = vk::createInstanceUnique( instanceCreateInfo );
    L_DEBUG( "VulkanInstance successfully created" );
}

VulkanInstance::~VulkanInstance() {}

vk::Instance &VulkanInstance::getInstance() { return *m_instance; }