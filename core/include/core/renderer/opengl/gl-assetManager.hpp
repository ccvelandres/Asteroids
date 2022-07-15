#pragma once

#include <renderer/asset-manager.hpp>
#include "gl-renderer.hpp"
#include "gl-pipeline.hpp"
#include "gl-texture.hpp"
#include "gl-mesh.hpp"

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
