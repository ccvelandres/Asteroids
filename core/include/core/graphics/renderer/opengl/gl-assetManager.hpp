#pragma once

/**
 * @file core/renderer/opengl/gl-assetManager.hpp
 * @author Cedric Velandres (ccvelandres@gmail.com)
 * 
 * @addtogroup OpenGL
 * @{
 */

#include "../../asset-manager.hpp"
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

    AssetID loadAsset(const AssetType &type, const AssetName &name) override;

    OpenGLMesh     &getMesh(AssetID id) const;
    OpenGLPipeline &getPipeline(AssetID id) const;
    OpenGLTexture  &getTexture(AssetID id) const;
};

/** @} endgroup OpenGL */
