#pragma once

#include <unordered_map>
#include <vector>
#include <string>
#include <istream>

using AssetName  = std::string;
using AssetPaths = std::vector<std::string>;

class AssetInventory;

enum class AssetType
{
    Mesh,
    Pipeline,
    Texture
};

enum class AssetDefaults
{
    MESH_QUAD = 0x0,
    MESH_XY_QUAD,
    MESH_YZ_QUAD,
    MESH_ZX_QUAD,

    PIPELINE_DEFAULT = 0x100,
    PIPELINE_2D,
    PIPELINE_3D,

    TEXTURE_DEFAULT = 0x200
};

class AssetHandle
{
public:
    AssetHandle(const AssetHandle &)            = delete;
    AssetHandle &operator=(const AssetHandle &) = delete;
    AssetHandle(AssetHandle &&)                 = default;
    AssetHandle &operator=(AssetHandle &&)      = default;

    const int     key;
    std::istream &getStream();
    std::size_t   assetSize;

    friend AssetInventory;
};

class AssetInventory
{
private:
    using AssetList = std::unordered_map<std::string, AssetPaths>;
    std::unordered_map<AssetType, AssetList> cache;

    AssetInventory();
public:
    static AssetInventory &getInstance();
    ~AssetInventory();

    AssetInventory(const AssetInventory &)            = delete;
    AssetInventory &operator=(const AssetInventory &) = delete;
    AssetInventory(AssetInventory &&)                 = delete;
    AssetInventory &operator=(AssetInventory &&)      = delete;

    void loadInventory(const std::string &inventoryFile);

    AssetHandle open(const AssetType &type, const AssetName &name);

    AssetPaths resolvePath(const AssetType &type, const AssetName &name);
    AssetPaths resolvePath(const AssetType &type, const AssetDefaults &id);
};
