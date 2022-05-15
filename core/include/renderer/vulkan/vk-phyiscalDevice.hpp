#pragma once

#include <vulkan/vulkan.hpp>
#include <memory>

class VulkanPhysicalDevice
{
private:
    vk::PhysicalDevice m_physicalDevice;

protected:
public:
    VulkanPhysicalDevice(const vk::Instance &instance);
    ~VulkanPhysicalDevice();

    vk::PhysicalDevice &getPhysicalDevice();
    operator vk::PhysicalDevice() { return getPhysicalDevice(); }
};