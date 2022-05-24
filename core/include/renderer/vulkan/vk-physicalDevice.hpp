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
    vk::SampleCountFlagBits getMultisampling();
    vk::Format              getDepthFormat();

    operator vk::PhysicalDevice() { return getPhysicalDevice(); }
};