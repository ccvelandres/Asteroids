#include <renderer/vulkan/vk-context.hpp>
#include <renderer/vulkan/vk-rendercontext.hpp>
#include <renderer/vulkan/vk-utils.hpp>

#include <utils/logging.hpp>
#include <utils/profiler.hpp>

#include <array>

VulkanImage createMultisampleImage( const VulkanPhysicalDevice &physicalDevice,
                                    const VulkanDevice         &device,
                                    const VulkanSwapchain      &swapchain,
                                    const VulkanCommandPool    &commandPool )
{
    L_TAG( "createMultisampleImage" );
    const vk::Extent2D         extent = swapchain.getExtent();
    const vk::SurfaceFormatKHR format = swapchain.getFormat();

    L_TRACE( "Creating multisampled image" );
    return VulkanImage( physicalDevice,
                        device,
                        commandPool,
                        extent.width,
                        extent.height,
                        1,
                        physicalDevice.getMultisampling(),
                        format.format,
                        vk::ImageTiling::eOptimal,
                        vk::ImageUsageFlagBits::eTransientAttachment | vk::ImageUsageFlagBits::eColorAttachment,
                        vk::MemoryPropertyFlagBits::eDeviceLocal,
                        vk::ImageLayout::eUndefined,
                        vk::ImageLayout::eColorAttachmentOptimal );
}

VulkanImageView createImageView( const VulkanDevice         &device,
                                 const VulkanImage          &image,
                                 const vk::ImageAspectFlags &aspectFlags )
{
    L_TAG( "createImageView" );
    L_TRACE( "Creating imageView" );
    return VulkanImageView( device, image.getImage(), image.getFormat(), aspectFlags, image.getMipLevels() );
}

VulkanImage createDepthImage( const VulkanPhysicalDevice &physicalDevice,
                              const VulkanDevice         &device,
                              const VulkanSwapchain      &swapchain,
                              const VulkanCommandPool    &commandPool )
{
    L_TAG( "createDepthImage" );
    const vk::Extent2D &extent{ swapchain.getExtent() };

    L_TRACE( "Creating depth image" );
    return VulkanImage( physicalDevice,
                        device,
                        commandPool,
                        extent.width,
                        extent.height,
                        1,
                        physicalDevice.getMultisampling(),
                        physicalDevice.getDepthFormat(),
                        vk::ImageTiling::eOptimal,
                        vk::ImageUsageFlagBits::eDepthStencilAttachment,
                        vk::MemoryPropertyFlagBits::eDeviceLocal,
                        vk::ImageLayout::eUndefined,
                        vk::ImageLayout::eDepthStencilAttachmentOptimal );
}

std::vector<vk::UniqueFramebuffer> createFramebuffers( const VulkanDevice     &device,
                                                       const VulkanSwapchain  &swapchain,
                                                       const VulkanRenderPass &renderpass,
                                                       const VulkanImageView  &multisampleImageView,
                                                       const VulkanImageView  &depthImageView )
{
    L_TAG( "createFramebuffers" );
    std::vector<vk::UniqueFramebuffer> framebuffers;

    const vk::Extent2D                  extent              = swapchain.getExtent();
    const std::vector<VulkanImageView> &swapchainImageViews = swapchain.getImageViews();

    L_TRACE( "Creating framebuffers for the swapchain images" );
    for ( const auto &swapchainImageView : swapchainImageViews )
    {
        std::array<vk::ImageView, 3> attachments = { multisampleImageView.getImageView(),
                                                     depthImageView.getImageView(),
                                                     swapchainImageView.getImageView() };

        vk::FramebufferCreateInfo framebufferCreateInfo( vk::FramebufferCreateFlags(),
                                                         renderpass.getRenderPass(),
                                                         static_cast<uint32_t>( attachments.size() ),
                                                         attachments.data(),
                                                         extent.width,
                                                         extent.height,
                                                         1 );

        framebuffers.push_back( device.getDevice().createFramebufferUnique( framebufferCreateInfo ) );
    }
    return framebuffers;
}

vk::Rect2D createScissor( const VulkanSwapchain &swapchain )
{
    vk::Offset2D offset( 0, 0 );

    return vk::Rect2D( offset, swapchain.getExtent() );
}

vk::Viewport createViewport( const VulkanSwapchain &swapchain )
{
    vk::Extent2D extent         = swapchain.getExtent();
    const float  viewportWidth  = static_cast<float>( extent.width );
    const float  viewportHeight = static_cast<float>( extent.height );

    return vk::Viewport( 0.f, 0.f, viewportWidth, viewportHeight, 0.f, 1.f );
}

std::array<vk::ClearValue, 2> createClearValues()
{
    std::array<vk::ClearValue, 2> clearValues;
    clearValues [0].color =
        vk::ClearColorValue( std::array<float, 4>{ 128.f / 256.f, 128.f / 256.f, 128.f / 256.f, 1.f } );
    clearValues [1].depthStencil = vk::ClearDepthStencilValue( 1.f, 0 );

    return clearValues;
}

struct VulkanRenderContext::Internal
{
    const VulkanSwapchain                      swapchain;
    const VulkanRenderPass                     renderpass;
    const VulkanImage                          multisampleImage;
    const VulkanImageView                      multisampleImageView;
    const VulkanImage                          depthImage;
    const VulkanImageView                      depthImageView;
    const std::vector<vk::UniqueFramebuffer>   framebuffers;
    const std::vector<vk::UniqueCommandBuffer> commandBuffers;
    const uint32_t                             maxRenderFrames = 2;
    const std::vector<vk::UniqueSemaphore>     graphicsSemaphores;
    const std::vector<vk::UniqueSemaphore>     presentSemaphores;
    const std::vector<vk::UniqueFence>         graphicsFences;
    const vk::Rect2D                           scissor;
    const vk::Viewport                         viewport;
    const std::array<vk::ClearValue, 2>        clearValues;

    Internal( SDL_Window                 *window,
              const VulkanInstance       &instance,
              const VulkanPhysicalDevice &physicalDevice,
              const VulkanDevice         &device,
              const VulkanSurface        &surface,
              const VulkanCommandPool    &commandPool )
        : swapchain( window, instance, physicalDevice, surface, device ),
          renderpass( physicalDevice, device, swapchain ),
          multisampleImage( ::createMultisampleImage( physicalDevice, device, swapchain, commandPool ) ),
          multisampleImageView( ::createImageView( device, multisampleImage, vk::ImageAspectFlagBits::eColor ) ),
          depthImage( ::createDepthImage( physicalDevice, device, swapchain, commandPool ) ),
          depthImageView( ::createImageView( device, depthImage, vk::ImageAspectFlagBits::eDepth ) ),
          framebuffers( ::createFramebuffers( device, swapchain, renderpass, multisampleImageView, depthImageView ) ),
          commandBuffers( commandPool.createCommandBuffers( device, swapchain.getImageCount() ) ),
          graphicsSemaphores( device.createSemaphores( maxRenderFrames ) ),
          presentSemaphores( device.createSemaphores( maxRenderFrames ) ),
          graphicsFences( device.createFences( maxRenderFrames ) ),
          scissor( ::createScissor( swapchain ) ),
          viewport( ::createViewport( swapchain ) ),
          clearValues( ::createClearValues() )
    {
        L_TAG( "VulkanRenderContext::Internal" );
        L_TRACE( "Internal resources initialized ({})", static_cast<void *>( this ) );
    }

    ~Internal()
    {
        L_TAG( "VulkanRenderContext::~Internal" );
        L_TRACE( "Internal resources freed ({})", static_cast<void *>( this ) );
    }
};

VulkanRenderContext::VulkanRenderContext( SDL_Window                 *window,
                                          const VulkanInstance       &instance,
                                          const VulkanPhysicalDevice &physicalDevice,
                                          const VulkanDevice         &device,
                                          const VulkanSurface        &surface,
                                          const VulkanCommandPool    &commandPool )
    : m_internal( std::make_unique<Internal>( window, instance, physicalDevice, device, surface, commandPool ) )
{
    L_TAG( "VulkanRenderContext::VulkanRenderContext" );
}

VulkanRenderContext::~VulkanRenderContext() = default;
