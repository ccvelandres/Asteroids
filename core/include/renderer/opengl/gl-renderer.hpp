#pragma once

#include <memory>
#include <renderer/asset-manager.hpp>
#include <renderer/renderer.hpp>

class OpenGLRenderer : public Renderer
{
private:
    struct Internal;
    std::unique_ptr<Internal> m_internal;
protected:
public:
    OpenGLRenderer(const std::string &windowTitle, const int windowHeight = 1280, const int windowWidth = 720);
    OpenGLRenderer(OpenGLRenderer &&o);
    OpenGLRenderer &operator=(OpenGLRenderer &&o);
    ~OpenGLRenderer();

    void init() override;
    void update(const time_ms delta) override;
    void clean() override;
    void refresh() override;

    bool renderBegin() override;
    void render(const std::vector<assets::Mesh> &meshes) override;
    void renderEnd() override;

    AssetManager& getAssetManager() override;
};