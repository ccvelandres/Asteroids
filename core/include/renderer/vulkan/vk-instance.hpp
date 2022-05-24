#pragma once

#include <memory>
#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>
#include <vulkan/vulkan.hpp>

class VulkanInstance
{
private:
    vk::UniqueInstance m_instance;

    static std::vector<const char *> getValidationLayers();
    static std::vector<const char *> getRequiredExtensions( SDL_Window *window );
protected:
public:
    VulkanInstance( SDL_Window *window );
    ~VulkanInstance();

    vk::Instance &getInstance();

    operator vk::Instance() { return getInstance(); }
};