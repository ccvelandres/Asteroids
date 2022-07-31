#include <core/assets/asset-inventory.hpp>
#include <core/utils/logging.hpp>

#include <filesystem>
#include <fstream>

AssetInventory &AssetInventory::getInstance() {
    static AssetInventory instance;
    return instance;
}

AssetInventory::AssetInventory() {
    L_TAG("AssetInventory()");

    /** load defaults without inventory file */
    cache[AssetType::Mesh]["crate"].push_back("assets/models/crate.obj");
    cache[AssetType::Mesh]["defaults/quad"].push_back("defaults/mesh/quad");
    cache[AssetType::Pipeline]["default"].push_back("shaders/opengl/default.vert");
    cache[AssetType::Pipeline]["default"].push_back("shaders/opengl/default.frag");
    cache[AssetType::Texture]["crate"].push_back("assets/textures/crate.png");
    cache[AssetType::Texture]["ship.png"].push_back("assets/textures/ship.png");
    cache[AssetType::Texture]["default"].push_back("assets/textures/default.png");

    L_TRACE("Internal resources initialized ({})", static_cast<void *>(this));
}

AssetInventory::~AssetInventory() = default;

void AssetInventory::loadInventory(const std::string &inventoryFile) {
    L_UNIMPLEMENTED("AssetInventory::loadInventory(&filename)");

    /** load inventory to cache */
}

AssetPaths AssetInventory::resolvePath(const AssetType &type, const std::string &name) { return cache[type][name]; }
