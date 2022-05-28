#pragma once

#include <memory>
#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>
#include <vulkan/vulkan.hpp>

class VulkanInstance
{
private:
    const vk::UniqueInstance m_instance;

protected:
public:
    VulkanInstance( SDL_Window *window );
    ~VulkanInstance();

    const vk::Instance &getInstance() const;

    operator vk::Instance() { return getInstance(); }
};