#pragma once

#include <memory>
#include <vulkan/vulkan.hpp>

#include "vk-instance.hpp"

class VulkanPhysicalDevice
{
private:
    struct Internal;
    std::unique_ptr<Internal> m_internal;

protected:
public:
    VulkanPhysicalDevice( const VulkanInstance &instance );
    ~VulkanPhysicalDevice();

    const vk::PhysicalDevice     &getPhysicalDevice() const;
    const vk::SampleCountFlagBits getMultisampling() const;
    const vk::Format              getDepthFormat() const;
    uint32_t                      getMemoryTypeIndex( const uint32_t                 filter,
                                                      const vk::MemoryPropertyFlags &flags );

    operator vk::PhysicalDevice() { return getPhysicalDevice(); }
};