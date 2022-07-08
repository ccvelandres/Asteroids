#include <assets/asset-inventory.hpp>

namespace AssetInventory
{
    /** @todo: Add thread-protection */
    struct Inventory
    {
        std::unordered_map<std::string, AssetList> cache;
    };

    std::shared_ptr<Inventory> inventory(std::make_shared<Inventory>());

    int loadInventory()
    {
        /** @todo: create program for creating inventory file */
        /** @todo: do proper parsing of inventory file */
        Inventory *ivn = new Inventory();

        ivn->cache["mesh"]["crate"].push_back("assets/models/crate.obj");
        ivn->cache["pipeline"]["default"].push_back("shaders/opengl/default.vert");
        ivn->cache["pipeline"]["default"].push_back("shaders/opengl/default.frag");

        inventory.reset(ivn);
    }

    int loadInventory(const std::string &filename){}

    AssetPaths resolvePath(const std::string &type, const std::string &name) { return inventory->cache[type][name]; }
} // namespace AssetInventory