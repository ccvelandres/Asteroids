#pragma once

/**
 * @file core/renderer/opengl/gl-assetManager.hpp
 * @author Cedric Velandres (ccvelandres@gmail.com)
 * 
 * @addtogroup OpenGL
 * @{
 */

#include <renderer/asset-manager.hpp>
#include "gl-renderer.hpp"
#include "gl-pipeline.hpp"
#include "gl-texture.hpp"
#include "gl-mesh.hpp"

/**
 * @brief OpenGL implementation of the AssetManager
 * 
 */
class OpenGLAssetManager : public AssetManager
{
private:
    struct Internal;
    std::unique_ptr<Internal> m_internal;
protected:
public:
    OpenGLAssetManager();
    OpenGLAssetManager(OpenGLAssetManager &&o);
    OpenGLAssetManager &operator=(OpenGLAssetManager &&o);
    ~OpenGLAssetManager();

    AssetID loadPipeline(const std::string &name) override;
    AssetID loadMesh(const std::string &name) override;
    AssetID loadTexture(const std::string &name) override;

    OpenGLMesh     &getMesh(AssetID id);
    OpenGLPipeline &getPipeline(AssetID id);
    OpenGLTexture  &getTexture(AssetID id);
};

/** @} endgroup OpenGL */
