#pragma once

#include <vulkan/vulkan.hpp>

#include "vk-commandpool.hpp"
#include "vk-device.hpp"
#include "vk-instance.hpp"
#include "vk-physicalDevice.hpp"
#include "vk-rendercontext.hpp"
#include "vk-surface.hpp"

class VulkanContext
{
private:
    struct Internal;
    std::unique_ptr<Internal> m_internal;

protected:
public:
    VulkanContext(VulkanContext &&o);
    VulkanContext &operator=(VulkanContext &&o);
    VulkanContext(SDL_Window *window);
    ~VulkanContext();

    bool renderBegin() const;
    void renderEnd() const;
};