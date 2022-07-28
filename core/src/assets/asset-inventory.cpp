#include <assets/asset-inventory.hpp>

namespace AssetInventory
{
    using AssetList  = std::unordered_map<std::string, AssetPaths>;

    /** @todo: Add thread-protection */
    struct Inventory
    {
        std::unordered_map<AssetType, AssetList> cache;
    };

    static Inventory inventory;

    int loadInventory()
    {
        /** @todo: create program for creating inventory file */
        /** @todo: do proper parsing of inventory file */
        inventory.cache.clear();

        inventory.cache[AssetType::Mesh]["crate"].push_back("assets/models/crate.obj");
        inventory.cache[AssetType::Pipeline]["default"].push_back("shaders/opengl/default.vert");
        inventory.cache[AssetType::Pipeline]["default"].push_back("shaders/opengl/default.frag");
        inventory.cache[AssetType::Texture]["crate"].push_back("assets/textures/crate.png");

        return EXIT_SUCCESS;
    }

    int loadInventory(const std::string &filename){ return EXIT_FAILURE; }

    AssetPaths resolvePath(const AssetType &type, const std::string &name) { return inventory.cache[type][name]; }
} // namespace AssetInventory