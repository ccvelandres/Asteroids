#pragma once

#include <memory>
#include <vulkan/vulkan.hpp>

#include "vk-instance.hpp"

class VulkanPhysicalDevice
{
private:
    vk::PhysicalDevice m_physicalDevice;

protected:
public:
    VulkanPhysicalDevice( VulkanInstance &instance );
    ~VulkanPhysicalDevice();

    vk::PhysicalDevice     &getPhysicalDevice();
    vk::Format              getDepthFormat();
    vk::SampleCountFlagBits getMultisampling();
    uint32_t getMemoryTypeIndex( const uint32_t filter, const vk::MemoryPropertyFlags &flags );

    operator vk::PhysicalDevice() { return getPhysicalDevice(); }
};