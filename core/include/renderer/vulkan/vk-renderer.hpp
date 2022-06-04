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
    VulkanRenderer( VulkanRenderer && );
    VulkanRenderer &operator=( VulkanRenderer &&other );
    VulkanRenderer( SDL_Window *window );
    ~VulkanRenderer();

    void init() override;
    void update( const time_ms delta ) override;
    void clean() override;
    void refresh() override;

    bool renderBegin() override;
    void render( const std::vector<Mesh> &meshes ) override;
    void renderEnd() override;
};