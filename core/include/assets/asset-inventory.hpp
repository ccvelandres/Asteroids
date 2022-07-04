#pragma once

#include "mesh.hpp"
#include "texture.hpp"

#include <unordered_map>
#include <vector>
#include <string>

namespace AssetInventory {
    using AssetPaths = std::vector<std::string>;
    using AssetList  = std::unordered_map<std::string, AssetPaths>;
    
    int loadInventory();
    int loadInventory(const std::string &filename);

    AssetPaths resolvePath(const std::string &type, const std::string &name);


}