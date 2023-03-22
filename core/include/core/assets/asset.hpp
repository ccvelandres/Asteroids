#pragma once

/**
 * @file core/renderer/asset.hpp
 * @author Cedric Velandres (ccvelandres@gmail.com)
 *
 * @addtogroup Assets
 * @{
 */

#include "../assets/asset-inventory.hpp"

namespace core::assets
{
    class Asset
    {
    private:
    protected:
    public:
        Asset() = default;
        virtual ~Asset(){};

        Asset(const Asset &)            = default;
        Asset &operator=(const Asset &) = default;
        Asset(Asset &&)                 = default;
        Asset &operator=(Asset &&)      = default;

        virtual int importAsset(const AssetName &name) = 0;
        virtual int exportAsset(const AssetName &name) = 0;
    };
} // namespace core::assets

/** @} endgroup Assets */