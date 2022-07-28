#pragma once

#include <memory>
#include <vulkan/vulkan.hpp>

#include "../../asset-manager.hpp"
#include "../renderer.hpp"

class VulkanRenderer : public Renderer
{
private:
    struct Internal;
    std::unique_ptr<Internal> m_internal;

protected:
public:
    VulkanRenderer(VulkanRenderer &&o);
    VulkanRenderer &operator=(VulkanRenderer &&o);
    VulkanRenderer(SDL_Window *window);
    ~VulkanRenderer();

    void init() override;
    void update(const time_ms delta) override;
    void clean() override;
    void refresh() override;

    bool renderBegin() override;
    void render() override;
    void renderEnd() override;
};