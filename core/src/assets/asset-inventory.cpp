#include <core/assets/asset-inventory.hpp>
#include <core/utils/logging.hpp>
#include <core/utils/platform.hpp>

#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

/** @todo: dynamic scan of asset folder or have a manifest file? */

AssetInventory &AssetInventory::getInstance()
{
    static AssetInventory instance;
    return instance;
}

AssetInventory::AssetInventory()
{
    L_TAG("AssetInventory()");

    // Set asset directory
    fs::path projectDirectory = core::utils::platform::getProjectPath();
    assetsDirectory           = fs::path(projectDirectory).append("assets").string();

    const std::unordered_map<std::string, std::pair<bool, AssetType>> assetTypeMap = {
        {"audio",        {false, AssetType::Audio}      }, // no strip extension
        {"models",       {false, AssetType::Mesh}       }, // no strip extension
        {"shaders",      {true, AssetType::Pipeline}    }, // strip extension
        {"textures",     {false, AssetType::Texture}    }, // no strip extension
        {"spritesheets", {false, AssetType::SpriteSheet}}, // no strip extension
        {"fonts",        {false, AssetType::Fonts}      }  // no strip extension
    };

    // Scan assets folder
    L_TRACE("Scanning {}", assetsDirectory);
    for (auto &dir : fs::directory_iterator(assetsDirectory))
    {
        if (!dir.is_directory()) continue;
        std::string assetDir = dir.path().stem().string();

        auto res = assetTypeMap.find(assetDir);
        if (res != assetTypeMap.end())
        {
            L_TRACE("\tScanning subdirectory: {}", assetDir);
            for (const auto &path : fs::recursive_directory_iterator(dir.path()))
            {
                if (path.is_directory()) continue;
                std::string assetPath = fs::relative(path.path(), projectDirectory).string();
                std::string assetName = fs::relative(path.path(), dir.path()).string();
                // Strip extension if needed
                if (res->second.first)
                {
                    assetName.erase(assetName.find_last_of('.'));
                }

                L_TRACE("\t\t{}: {}", assetName, assetPath);
                cache[res->second.second][assetName].push_back(assetPath);
            }
        }
    }

    L_TRACE("Internal resources initialized ({})", static_cast<void *>(this));
}

AssetInventory::~AssetInventory() = default;

void AssetInventory::loadInventory(const std::string &inventoryFile)
{
    L_UNIMPLEMENTED("AssetInventory::loadInventory(&filename)");

    /** load inventory to cache */
}

const std::vector<AssetPath> &AssetInventory::lookupAssets(const AssetType &type,
                                                           const AssetName &name)
{
    L_TAG("AssetInventory::lookupAssets");

    auto assetList = cache.find(type);
    if (assetList != cache.end())
    {
        auto assetPath = assetList->second.find(name);
        if (assetPath != assetList->second.end())
        {
            return assetPath->second;
        }
    }

    L_THROW_RUNTIME("Could not find matching assets");
}