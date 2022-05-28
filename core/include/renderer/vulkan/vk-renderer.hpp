#pragma once

#include <memory>
#include <renderer.hpp>
#include <vulkan/vulkan.hpp>

class VulkanRenderer : public BaseRenderer
{
private:
    struct Internal;
    std::unique_ptr<Internal> m_internal;

protected:
public:
    VulkanRenderer( SDL_Window *window );
    ~VulkanRenderer();
};