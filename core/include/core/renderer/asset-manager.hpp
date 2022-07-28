#pragma once

/**
 * @file core/renderer/asset-manager.hpp
 * @author Cedric Velandres (ccvelandres@gmail.com)
 * 
 * @addtogroup Renderer
 * @{
 */

#include "../assets/asset-inventory.hpp"

using AssetID = std::size_t;

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

    virtual AssetID loadPipeline(const std::string &name) = 0;
    virtual AssetID loadMesh(const std::string &name)     = 0;
    virtual AssetID loadTexture(const std::string &name)  = 0;
};

/** @} endgroup Renderer */
