#include <renderer/vulkan/vk-rendercontext.hpp>
#include <renderer/vulkan/vk-renderpass.hpp>
#include <renderer/vulkan/vk-utils.hpp>

#include <utils/logging.hpp>

VulkanRenderPass::VulkanRenderPass( VulkanPhysicalDevice &physicalDevice,
                                    VulkanDevice         &device,
                                    VulkanSwapchain      &swapchain )
{
    L_TAG( "VulkanRenderPass::VulkanRenderPass" );

    vk::Format              depthFormat   = physicalDevice.getDepthFormat();
    vk::Format              colorFormat   = swapchain.getFormat().format;
    vk::SampleCountFlagBits multisampling = physicalDevice.getMultisampling();

    vk::AttachmentDescription colorAttachment =
        vk::AttachmentDescription( vk::AttachmentDescriptionFlags(),
                                   colorFormat,
                                   multisampling,
                                   vk::AttachmentLoadOp::eClear,
                                   vk::AttachmentStoreOp::eStore,
                                   vk::AttachmentLoadOp::eDontCare,
                                   vk::AttachmentStoreOp::eDontCare,
                                   vk::ImageLayout::eUndefined,
                                   vk::ImageLayout::eColorAttachmentOptimal );

    vk::AttachmentReference colorAttachmentReference =
        vk::AttachmentReference( 0, vk::ImageLayout::eColorAttachmentOptimal );

    vk::AttachmentDescription depthAttachment =
        vk::AttachmentDescription( vk::AttachmentDescriptionFlags(),
                                   depthFormat,
                                   multisampling,
                                   vk::AttachmentLoadOp::eClear,
                                   vk::AttachmentStoreOp::eDontCare,
                                   vk::AttachmentLoadOp::eDontCare,
                                   vk::AttachmentStoreOp::eDontCare,
                                   vk::ImageLayout::eUndefined,
                                   vk::ImageLayout::eDepthStencilAttachmentOptimal );

    vk::RenderPassCreateInfo renderPassCreateInfo;
}

VulkanRenderPass::~VulkanRenderPass() {}
