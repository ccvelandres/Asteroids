#pragma once

/**
 * @file core/renderer/opengl/gl-renderer.hpp
 * @author Cedric Velandres (ccvelandres@gmail.com)
 * 
 * @defgroup OpenGL
 * @brief OpenGL implementation for the Rendering System
 * 
 * The OpenGLRenderer supports minimum of GLES 1.2 
 * 
 * @ingroup Renderer
 * @{
 */

#include <memory>

#include "../../asset-manager.hpp"
#include "core/graphics/renderer.hpp"

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
    void render() override;
    void renderEnd() override;

    AssetManager& getAssetManager() override;
};

/** @} endgroup OpenGL */
