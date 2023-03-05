#pragma once

/**
 * @file core/assets/texture.hpp
 * @author Cedric Velandres (ccvelandres@gmail.com)
 *
 * @addtogroup Assets
 * @{
 */

#include <string>
#include <memory>

#include "asset-inventory.hpp"

namespace core::assets
{
    /** Forward declaration for Model */
    class Model;

    /**
     * @brief Helper class for loading texture files
     *
     */
    class Texture
    {
    private:
        struct Internal;
        std::unique_ptr<Internal> m_internal;
    protected:
    public:
        Texture();
        Texture(const AssetName &name);
        ~Texture();

        Texture(Texture &o)             = delete;
        Texture &operator=(Texture &o)  = delete;
        Texture(Texture &&o)            = default;
        Texture &operator=(Texture &&o) = default;

        /** @brief Returns the texture name */
        const std::string &name() const noexcept;
        /** @brief Returns the sdl surface */
        const Internal& getInternal() const noexcept;

        friend Model;
    };
} // namespace core::assets

/** @} endgroup Assets */
