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
    public:
        struct Internal;
    private:
        std::unique_ptr<Internal> m_internal;
    protected:
    public:
        Texture();
        Texture(const AssetName &name);
        ~Texture();

        Texture(Texture &o)            = delete;
        Texture &operator=(Texture &o) = delete;
        Texture(Texture &&o);
        Texture &operator=(Texture &&o);

        /** @brief Returns the texture name */
        const std::string &name() const noexcept;
        /** @brief Returns the size of texture in bytes */
        std::size_t size() const noexcept;
        /** @brief Returns the internal pointer */
        const Internal &getInternal() const noexcept;

        friend Model;
    };
} // namespace core::assets

/** @} endgroup Assets */
