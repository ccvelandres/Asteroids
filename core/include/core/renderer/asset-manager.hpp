#pragma once

#include "../assets/asset-inventory.hpp"

using AssetID = std::size_t;

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