#pragma once

#include "mesh.hpp"
#include "texture.hpp"

#include <unordered_map>
#include <vector>
#include <string>

using AssetName = std::string;
using AssetPaths = std::vector<std::string>;

enum class AssetType {
    Mesh,
    Pipeline,
    Texture
};

namespace AssetInventory {

    int loadInventory();
    int loadInventory(const std::string &filename);

    AssetPaths resolvePath(const AssetType &type, const std::string &name);
}