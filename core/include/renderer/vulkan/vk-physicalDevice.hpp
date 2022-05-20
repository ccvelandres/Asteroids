#pragma once

#include <vulkan/vulkan.hpp>
#include <memory>

#include "vk-instance.hpp"

class VulkanPhysicalDevice
{
private:
    vk::PhysicalDevice m_physicalDevice;

protected:
public:
    VulkanPhysicalDevice(VulkanInstance &instance);
    ~VulkanPhysicalDevice();

    vk::PhysicalDevice &getPhysicalDevice();
    operator vk::PhysicalDevice() { return getPhysicalDevice(); }

    vk::SampleCountFlagBits getMultisampling();
    vk::Format getDepthFormat();
};