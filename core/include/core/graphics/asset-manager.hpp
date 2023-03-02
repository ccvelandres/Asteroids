#pragma once

/**
 * @file core/renderer/asset-manager.hpp
 * @author Cedric Velandres (ccvelandres@gmail.com)
 * 
 * @addtogroup Graphics
 * @{
 */

#include "../assets/asset-inventory.hpp"
#include "../assets/mesh.hpp"
#include "../assets/texture.hpp"
#include "../assets/shader.hpp"

using AssetID = std::intmax_t;

/**
 * @brief Base class for Asset Managers responsible for managing game assets
 * used by renderer implementations
 * 
 */
class AssetManager
{
private:
protected:
public:
    AssetManager()                            = default;
    AssetManager(AssetManager &o)             = delete;
    AssetManager &operator=(AssetManager &o)  = delete;
    AssetManager(AssetManager &&o)            = default;
    AssetManager &operator=(AssetManager &&o) = default;
    virtual ~AssetManager()                   = default;

    virtual AssetID loadAsset(const AssetType &type, const AssetName &name) = 0;

    virtual AssetID loadMesh(const core::assets::Mesh &mesh) = 0;
    virtual AssetID loadTexture(const core::assets::Texture &texture) = 0;
    virtual AssetID loadPipeline(const core::assets::Shader &shader) = 0;
};

/** @} endgroup Renderer */
