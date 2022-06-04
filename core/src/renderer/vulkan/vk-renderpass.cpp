#include <renderer/vulkan/vk-rendercontext.hpp>
#include <renderer/vulkan/vk-renderpass.hpp>
#include <renderer/vulkan/vk-utils.hpp>

#include <utils/logging.hpp>

vk::UniqueRenderPass createRenderPass( const VulkanPhysicalDevice &physicalDevice,
                                       const VulkanDevice         &device,
                                       const VulkanSwapchain      &swapchain )
{
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
    vk::AttachmentReference depthAttachmentReference =
        vk::AttachmentReference( 1, vk::ImageLayout::eDepthStencilAttachmentOptimal );

    vk::AttachmentDescription multisamplingAttachment =
        vk::AttachmentDescription( vk::AttachmentDescriptionFlags(),
                                   colorFormat,
                                   vk::SampleCountFlagBits::e1,
                                   vk::AttachmentLoadOp::eDontCare,
                                   vk::AttachmentStoreOp::eStore,
                                   vk::AttachmentLoadOp::eDontCare,
                                   vk::AttachmentStoreOp::eDontCare,
                                   vk::ImageLayout::eUndefined,
                                   vk::ImageLayout::ePresentSrcKHR );
    vk::AttachmentReference multisamplingAttachmentReference =
        vk::AttachmentReference( 2, vk::ImageLayout::eColorAttachmentOptimal );

    std::array<vk::AttachmentDescription, 3> attachments = { colorAttachment,
                                                             depthAttachment,
                                                             multisamplingAttachment };

    vk::SubpassDescription subpassDescription =
        vk::SubpassDescription( vk::SubpassDescriptionFlags(),
                                vk::PipelineBindPoint::eGraphics,
                                0,
                                nullptr,
                                1,
                                &colorAttachmentReference,
                                &multisamplingAttachmentReference,
                                &depthAttachmentReference,
                                0,
                                nullptr );

    vk::SubpassDependency subpassDependencies =
        vk::SubpassDependency( 0,
                               0,
                               vk::PipelineStageFlagBits::eColorAttachmentOutput,
                               vk::PipelineStageFlagBits::eColorAttachmentOutput,
                               vk::AccessFlags(),
                               vk::AccessFlagBits::eColorAttachmentRead
                                   | vk::AccessFlagBits::eColorAttachmentWrite,
                               vk::DependencyFlags() );

    vk::RenderPassCreateInfo renderPassCreateInfo =
        vk::RenderPassCreateInfo( vk::RenderPassCreateFlags(),
                                  static_cast<uint32_t>( attachments.size() ),
                                  attachments.data(),
                                  1,
                                  &subpassDescription,
                                  1,
                                  &subpassDependencies );

    return device.getDevice().createRenderPassUnique( renderPassCreateInfo );
}

struct VulkanRenderPass::Internal
{
    const vk::UniqueRenderPass renderpass;

    Internal( const VulkanPhysicalDevice &physicalDevice,
                                    const VulkanDevice         &device,
                                    const VulkanSwapchain      &swapchain )
    : renderpass( ::createRenderPass( physicalDevice, device, swapchain ) )
    {
        L_TAG( "VulkanRenderPass::Internal" );
        L_TRACE( "Internal resources initialized ({})", static_cast<void*>(this) );
    }

    ~Internal()
    {
        L_TAG( "VulkanRenderPass::~Internal" );
        L_TRACE( "Internal resources freed ({})", static_cast<void*>(this) );
    }
};

VulkanRenderPass::VulkanRenderPass( const VulkanPhysicalDevice &physicalDevice,
                                    const VulkanDevice         &device,
                                    const VulkanSwapchain      &swapchain )
    : m_internal( std::make_unique<Internal>( physicalDevice, device, swapchain ) )
{
}

VulkanRenderPass::VulkanRenderPass( VulkanRenderPass &&o ) = default;
VulkanRenderPass &VulkanRenderPass::operator=( VulkanRenderPass &&o ) = default;
VulkanRenderPass::~VulkanRenderPass() = default;

const vk::RenderPass &VulkanRenderPass::getRenderPass() const { return *m_internal->renderpass; }