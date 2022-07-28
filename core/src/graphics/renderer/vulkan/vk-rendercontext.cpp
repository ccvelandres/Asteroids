#include <core/graphics/renderer/vulkan/vk-context.hpp>
#include <core/graphics/renderer/vulkan/vk-rendercontext.hpp>
#include <core/graphics/renderer/vulkan/vk-utils.hpp>

#include <utils/logging.hpp>
#include <utils/profiler.hpp>

#include <array>

VulkanImage createMultisampleImage(const VulkanPhysicalDevice &physicalDevice,
                                   const VulkanDevice         &device,
                                   const VulkanSwapchain      &swapchain,
                                   const VulkanCommandPool    &commandPool)
{
    L_TAG("createMultisampleImage");
    const vk::Extent2D         extent = swapchain.getExtent();
    const vk::SurfaceFormatKHR format = swapchain.getFormat();

    L_TRACE("Creating multisampled image");
    return VulkanImage(physicalDevice,
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
                       vk::ImageLayout::eColorAttachmentOptimal);
}

VulkanImageView createImageView(const VulkanDevice         &device,
                                const VulkanImage          &image,
                                const vk::ImageAspectFlags &aspectFlags)
{
    L_TAG("createImageView");
    L_TRACE("Creating imageView");
    return VulkanImageView(device, image.getImage(), image.getFormat(), aspectFlags, image.getMipLevels());
}

VulkanImage createDepthImage(const VulkanPhysicalDevice &physicalDevice,
                             const VulkanDevice         &device,
                             const VulkanSwapchain      &swapchain,
                             const VulkanCommandPool    &commandPool)
{
    L_TAG("createDepthImage");
    const vk::Extent2D &extent{swapchain.getExtent()};

    L_TRACE("Creating depth image");
    return VulkanImage(physicalDevice,
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
                       vk::ImageLayout::eDepthStencilAttachmentOptimal);
}

std::vector<vk::UniqueFramebuffer> createFramebuffers(const VulkanDevice     &device,
                                                      const VulkanSwapchain  &swapchain,
                                                      const VulkanRenderPass &renderPass,
                                                      const VulkanImageView  &multisampleImageView,
                                                      const VulkanImageView  &depthImageView)
{
    L_TAG("createFramebuffers");
    std::vector<vk::UniqueFramebuffer> framebuffers;

    const vk::Extent2D                  extent              = swapchain.getExtent();
    const std::vector<VulkanImageView> &swapchainImageViews = swapchain.getImageViews();

    L_TRACE("Creating framebuffers for the swapchain images");
    for (const auto &swapchainImageView : swapchainImageViews)
    {
        std::array<vk::ImageView, 3> attachments = {multisampleImageView.getImageView(),
                                                    depthImageView.getImageView(),
                                                    swapchainImageView.getImageView()};

        vk::FramebufferCreateInfo framebufferCreateInfo(vk::FramebufferCreateFlags(),
                                                        renderPass.getRenderPass(),
                                                        static_cast<uint32_t>(attachments.size()),
                                                        attachments.data(),
                                                        extent.width,
                                                        extent.height,
                                                        1);

        framebuffers.push_back(device.getDevice().createFramebufferUnique(framebufferCreateInfo));
    }
    return framebuffers;
}

vk::Rect2D createScissor(const VulkanSwapchain &swapchain)
{
    L_TAG("createScissor");

    vk::Offset2D offset(0, 0);

    L_TRACE("Creating scissors for viewport");
    return vk::Rect2D(offset, swapchain.getExtent());
}

vk::Viewport createViewport(const VulkanSwapchain &swapchain)
{
    L_TAG("createViewport");

    vk::Extent2D extent         = swapchain.getExtent();
    const float  viewportWidth  = static_cast<float>(extent.width);
    const float  viewportHeight = static_cast<float>(extent.height);

    L_TRACE("Creating viewport");
    return vk::Viewport(0.f, 0.f, viewportWidth, viewportHeight, 0.f, 1.f);
}

std::array<vk::ClearValue, 2> createClearValues()
{
    L_TAG("createClearValues");

    std::array<vk::ClearValue, 2> clearValues;
    clearValues[0].color = vk::ClearColorValue(std::array<float, 4>{128.f / 256.f, 128.f / 256.f, 128.f / 256.f, 1.f});
    clearValues[1].depthStencil = vk::ClearDepthStencilValue(1.f, 0);

    L_TRACE("Creating ClearValues");
    return clearValues;
}

uint32_t acquireNextImageIndex(const vk::Device       &device,
                               const vk::SwapchainKHR &swapchain,
                               const vk::Fence        &fence,
                               const vk::Semaphore    &semaphore)
{
    L_TAG("acquireNextImageIndex");

    static constexpr uint64_t timeout = std::numeric_limits<uint64_t>::max();

    /** Wait for fence */
    device.waitForFences(1, &fence, VK_TRUE, timeout);
    /** Reset fence to ready for next use case */
    device.resetFences(1, &fence);

    /** Grab the next image index and return index */
    return device.acquireNextImageKHR(swapchain, timeout, semaphore, nullptr).value;
}

struct VulkanRenderContext::Internal
{
    const VulkanSwapchain                      swapchain;
    const VulkanRenderPass                     renderPass;
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

    uint32_t currentFrameIndex          = 0;
    uint32_t currentSwapchainImageIndex = 0;

    Internal(SDL_Window *const           window,
             const VulkanInstance       &instance,
             const VulkanPhysicalDevice &physicalDevice,
             const VulkanDevice         &device,
             const VulkanSurface        &surface,
             const VulkanCommandPool    &commandPool,
             const vk::SwapchainKHR     &oldSwapchain)
        : swapchain(window, instance, physicalDevice, surface, device, oldSwapchain),
          renderPass(physicalDevice, device, swapchain),
          multisampleImage(::createMultisampleImage(physicalDevice, device, swapchain, commandPool)),
          multisampleImageView(::createImageView(device, multisampleImage, vk::ImageAspectFlagBits::eColor)),
          depthImage(::createDepthImage(physicalDevice, device, swapchain, commandPool)),
          depthImageView(::createImageView(device, depthImage, vk::ImageAspectFlagBits::eDepth)),
          framebuffers(::createFramebuffers(device, swapchain, renderPass, multisampleImageView, depthImageView)),
          commandBuffers(commandPool.createCommandBuffers(device, swapchain.getImageCount())),
          graphicsSemaphores(device.createSemaphores(maxRenderFrames)),
          presentSemaphores(device.createSemaphores(maxRenderFrames)),
          graphicsFences(device.createFences(maxRenderFrames)),
          scissor(::createScissor(swapchain)),
          viewport(::createViewport(swapchain)),
          clearValues(::createClearValues())
    {
        L_TAG("VulkanRenderContext::Internal");
        L_TRACE("Internal resources initialized ({})", static_cast<void *>(this));
    }

    ~Internal()
    {
        L_TAG("VulkanRenderContext::~Internal");
        L_TRACE("Internal resources freed ({})", static_cast<void *>(this));
    }

    bool renderBegin(const VulkanDevice &device)
    {
        L_TAG("VulkanRenderContext::Internal::renderBegin");

        /** Get fence and semaphore for current frame index */
        const vk::Fence     &graphicsFence     = graphicsFences[currentFrameIndex].get();
        const vk::Semaphore &graphicsSemaphore = graphicsSemaphores[currentFrameIndex].get();

        /** Enclose with try-catch since we expect stuff inside to throw if it fails */
        try
        {
            currentSwapchainImageIndex =
                ::acquireNextImageIndex(device.getDevice(), swapchain.getSwapchain(), graphicsFence, graphicsSemaphore);
        }
        catch (const vk::OutOfDateKHRError &e)
        {
            L_TRACE("Exception: cannot render with current context, need to recreate it ({})", e.what());
            return false;
        }
        catch (const std::exception &e)
        {
            L_THROW_RUNTIME("Unhandled exception: {}", e.what());
        }

        /** Grab command buffer for currentSwapchainImageIndex */
        const vk::CommandBuffer &commandBuffer = commandBuffers[currentSwapchainImageIndex].get();

        /** Reset the command buffer */
        commandBuffer.reset(vk::CommandBufferResetFlagBits::eReleaseResources);
        /** Begin the command buffer */
        commandBuffer.begin(vk::CommandBufferBeginInfo(vk::CommandBufferUsageFlagBits::eOneTimeSubmit, nullptr));

        /** Configure scissor and viewport */
        commandBuffer.setScissor(0, 1, &scissor);
        commandBuffer.setViewport(0, 1, &viewport);

        /** Define the render pass info */
        vk::RenderPassBeginInfo renderPassBeginInfo(renderPass.getRenderPass(),
                                                    framebuffers[currentFrameIndex].get(),
                                                    scissor,
                                                    2,
                                                    clearValues.data());
        /** Record the render pass begin command */
        commandBuffer.beginRenderPass(renderPassBeginInfo, vk::SubpassContents::eInline);

        /** Render context is now ready */
        return true;
    }

    bool renderEnd(const VulkanDevice &device)
    {
        L_TAG("VulkanRenderContext::Internal::renderEnd");

        /** Grab command buffer for currentSwapchainImageIndex */
        const vk::CommandBuffer &commandBuffer = commandBuffers[currentSwapchainImageIndex].get();

        /** Stop render pass and the command buffer */
        commandBuffer.endRenderPass();
        commandBuffer.end();

        /** Grab the fence, graphics and preset semaphore for current frame index */
        const vk::Fence             &graphicsFence      = graphicsFences[currentFrameIndex].get();
        const vk::Semaphore         &graphicsSemaphore  = graphicsSemaphores[currentFrameIndex].get();
        const vk::Semaphore         &presentSemaphore   = presentSemaphores[currentFrameIndex].get();
        const vk::PipelineStageFlags pipelineStageFlags = vk::PipelineStageFlagBits::eColorAttachmentOutput;

        /** Build submit info for graphics queue */
        vk::SubmitInfo submitInfo(1, &graphicsSemaphore, &pipelineStageFlags, 1, &commandBuffer, 1, &presentSemaphore);

        /** Submit command buffer and give the graphics fence */
        device.getGraphicsQueue().submit(1, &submitInfo, graphicsFence);

        /** Build the present info to preset the image to the display hardware */
        vk::PresentInfoKHR presentInfo(1,
                                       &presentSemaphore,
                                       1,
                                       &swapchain.getSwapchain(),
                                       &currentSwapchainImageIndex,
                                       nullptr);

        /** Enclose with try-catch since we expect stuff inside to throw if it fails */
        try
        {
            /** Try to submit the graphics output to the present queue to display it
             *  Return false if we receive an exception and/or eSuboptimalKHR
             *  meaning we need to recreate the render context
             */
            if (device.getPresentQueue().presentKHR(presentInfo) == vk::Result::eSuboptimalKHR) return false;
        }
        catch (const vk::OutOfDateKHRError &e)
        {
            L_TRACE("Exception: cannot present gprahics output with current context, need to recreate it ({})", e.what());
            return false;
        }
        catch (const std::exception &e)
        {
            L_THROW_RUNTIME("Unhandled exception: {}", e.what());
        }

        /** Now wait till the presentation is complete, before continuing */
        /** @todo: maybe another semaphore to signal presentation is complete? so we can skip the wait */
        device.getPresentQueue().waitIdle();

        /** Now we increment the frame index */
        currentFrameIndex = (currentFrameIndex + 1) % maxRenderFrames;

        return true;
    }
};

VulkanRenderContext::VulkanRenderContext(SDL_Window *const           window,
                                         const VulkanInstance       &instance,
                                         const VulkanPhysicalDevice &physicalDevice,
                                         const VulkanDevice         &device,
                                         const VulkanSurface        &surface,
                                         const VulkanCommandPool    &commandPool,
                                         const vk::SwapchainKHR     &oldSwapchain)
    : m_internal(std::make_unique<Internal>(window, instance, physicalDevice, device, surface, commandPool, oldSwapchain))
{
    L_TAG("VulkanRenderContext::VulkanRenderContext");
}

VulkanRenderContext::VulkanRenderContext(VulkanRenderContext &&o)            = default;
VulkanRenderContext &VulkanRenderContext::operator=(VulkanRenderContext &&o) = default;
VulkanRenderContext::~VulkanRenderContext()                                  = default;

VulkanRenderContext VulkanRenderContext::recreate(SDL_Window *const           window,
                                                  const VulkanInstance       &instance,
                                                  const VulkanPhysicalDevice &physicalDevice,
                                                  const VulkanDevice         &device,
                                                  const VulkanSurface        &surface,
                                                  const VulkanCommandPool    &commandPool)
{
    return VulkanRenderContext(window,
                               instance,
                               physicalDevice,
                               device,
                               surface,
                               commandPool,
                               m_internal->swapchain.getSwapchain());
}

bool VulkanRenderContext::renderBegin(const VulkanDevice &device) const { return m_internal->renderBegin(device); }

bool VulkanRenderContext::renderEnd(const VulkanDevice &device) const { return m_internal->renderEnd(device); }
