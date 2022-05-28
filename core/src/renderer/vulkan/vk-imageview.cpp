#include <renderer/vulkan/vk-imageview.hpp>
#include <renderer/vulkan/vk-utils.hpp>
#include <SDL2/SDL_vulkan.h>

#include <utils/logging.hpp>

#include <algorithm>
#include <limits>

std::vector<vk::UniqueImageView> createImageViews( const VulkanDevice &device,
                                                   const VulkanSwapchain &swapchain )
{
    L_TAG( "createImageViews" );

    std::vector<vk::UniqueImageView> imageViews;
    const std::vector<vk::Image>    &images = swapchain.getImages();

    vk::ImageSubresourceRange
        imageSubresourceRange( vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1 );
    vk::ComponentMapping componentMapping( vk::ComponentSwizzle::eIdentity,
                                           vk::ComponentSwizzle::eIdentity,
                                           vk::ComponentSwizzle::eIdentity,
                                           vk::ComponentSwizzle::eIdentity );
    /** Reserve same size of images */
    imageViews.reserve( images.size() );

    /** Create an image view for each image */
    L_TRACE( "Creating image views for each swapchain image" );
    for ( const auto &image : images )
    {
        vk::ImageViewCreateInfo imageViewCreateInfo = {};
        imageViewCreateInfo.setImage( image )
            .setViewType( vk::ImageViewType::e2D )
            .setFormat( swapchain.getFormat().format )
            .setComponents( componentMapping )
            .setSubresourceRange( imageSubresourceRange );

        imageViews.push_back(
            device.getDevice().createImageViewUnique( imageViewCreateInfo ) );
    }

    return imageViews;
}

struct VulkanImageView::Internal
{
    const std::vector<vk::UniqueImageView> imageViews;

    Internal( const VulkanDevice &device, const VulkanSwapchain &swapchain )
        : imageViews( ::createImageViews( device, swapchain ) )
    {
        L_TAG( "VulkanImageView::Internal" );
        L_DEBUG( "ImageViews successfully created" );
    }
};

VulkanImageView::VulkanImageView( const VulkanDevice    &device,
                                  const VulkanSwapchain &swapchain )
    : m_internal( std::make_unique<Internal>( device, swapchain ) )
{
}

VulkanImageView::~VulkanImageView() {}

const vk::ImageView &VulkanImageView::getImageViews( const int index ) const
{
    L_TAG( "VulkanImageView::getImageViews" );
    const std::vector<vk::UniqueImageView> &imageViews = m_internal->imageViews;
    if ( index >= imageViews.size() )
    {
        L_THROW( std::out_of_range,
                 "MaxIndex: {} Argument: {}",
                 imageViews.size(),
                 index );
    }
    return *imageViews [index];
}
