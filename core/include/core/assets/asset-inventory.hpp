#pragma once

/**
 * @file core/assets/asset-inventory.cpp
 * @author Cedric Velandres (ccvelandres@gmail.com)
 *
 * I reaaaaaally don't like this current implementation :(
 * Ideally, i want to have a single interface for reading assets either from
 * a single file or a packed file or from memory
 *
 * @todo: refactor asset access
 * Specs:
 *  - Scan .pack files in assets, cache assets in packed file so we can access with name
 *  - support opening assets with filesystem paths
 *  - support reading assets from memory?
 *  - common interface wherever the asset is from (ideally as stream when opening from file)
 *      and no copying whole file to mem
 *  - maybe implement Asset class with streams? then have protected constructors for
 *      memory access and file access (with streambufs)
 *
 * @addtogroup Assets
 * @{
 */

#include <unordered_map>
#include <vector>
#include <string>
#include <fstream>

using AssetName  = std::string;
using AssetPath = std::string;
using AssetPaths = std::vector<AssetPath>;

class AssetInventory; /** Forward declaration for AssetInventory */

/**
 * @brief Enum class for Asset types
 *
 */
enum class AssetType
{
    Audio,
    Mesh,
    Model,
    Pipeline,
    Texture,
    SpriteSheet,
    Fonts
};

class AssetInventory
{
private:
    using AssetList = std::unordered_map<std::string, AssetPaths>;
    std::unordered_map<AssetType, AssetList> cache;
    std::string                              assetsDirectory;

    AssetInventory();
public:
    static AssetInventory &getInstance();
    ~AssetInventory();

    AssetInventory(const AssetInventory &)            = delete;
    AssetInventory &operator=(const AssetInventory &) = delete;
    AssetInventory(AssetInventory &&)                 = delete;
    AssetInventory &operator=(AssetInventory &&)      = delete;

    void loadInventory(const std::string &inventoryFile);

    const std::vector<AssetPath>& lookupAssets(const AssetType &type, const AssetName &name);
};

/** @} endgroup Assets */
