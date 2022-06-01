#pragma once

#include <memory>
#include <SDL2/SDL.h>
#include <vulkan/vulkan.hpp>

#include <time.hpp>

#include "vk-context.hpp"

class VulkanApplication
{
private:
    struct Internal;
    std::unique_ptr<Internal> m_internal;

protected:
public:
    VulkanApplication( SDL_Window                 *window );
    ~VulkanApplication();

    void update(const time_ms &delta);
    void render();
};