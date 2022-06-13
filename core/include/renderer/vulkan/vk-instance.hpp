#pragma once

#include <memory>
#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>
#include <vulkan/vulkan.hpp>

class VulkanInstance
{
private:
    struct Internal;
    std::unique_ptr<Internal> m_internal;

protected:
public:
    VulkanInstance(VulkanInstance &&o);
    VulkanInstance &operator=(VulkanInstance &&o);
    VulkanInstance(SDL_Window *window);
    ~VulkanInstance();

    const vk::Instance &getInstance() const;
};