#pragma once

#include <memory>
#include <renderer/common/renderer.hpp>
#include <vulkan/vulkan.hpp>

class VulkanRenderer : public Renderer
{
private:
    struct Internal;
    std::unique_ptr<Internal> m_internal;

protected:
public:
    VulkanRenderer( SDL_Window *window );
    ~VulkanRenderer();

    void init() override;
    void update(const time_ms delta) override;
    void clean() override;
    void refresh() override;

    void renderBegin() override;
    void render() override;
    void renderEnd() override;
};