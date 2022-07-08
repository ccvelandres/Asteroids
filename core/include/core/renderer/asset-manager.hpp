#pragma once

#include "../assets/asset-inventory.hpp"

using AssetID = std::size_t;

class AssetManager {
private:
protected:
public:

    virtual AssetID loadPipeline(const std::string &name) = 0;
    virtual AssetID loadMesh(const std::string &name) = 0;
    virtual AssetID loadTexture(const std::string &name) = 0;
};