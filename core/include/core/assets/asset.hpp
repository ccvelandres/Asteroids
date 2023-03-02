#pragma once

/**
 * @file core/renderer/asset.hpp
 * @author Cedric Velandres (ccvelandres@gmail.com)
 *
 * @addtogroup Graphics
 * @{
 */

#include "../assets/asset-inventory.hpp"

#include <memory>

namespace core::assets
{
    class Asset
    {
    private:
    protected:
    public:
        Asset(const AssetPath &path);
        ~Asset();

        Asset(const Asset &)            = delete;
        Asset &operator=(const Asset &) = delete;
        Asset(Asset &&)                 = default;
        Asset &operator=(Asset &&)      = default;

        std::vector<char> loadBinaryFile();
    };
} // namespace core::assets

/** @} endgroup Renderer */