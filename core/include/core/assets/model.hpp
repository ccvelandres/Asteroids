#pragma once

/**
 * @file core/assets/model.hpp
 * @author Cedric Velandres (ccvelandres@gmail.com)
 *
 * @addtogroup Assets
 * @{
 */

#include <string>
#include <vector>
#include <SDL_surface.h>

#include "asset-inventory.hpp"
#include "mesh.hpp"
#include "texture.hpp"

namespace core::assets
{
    /**
     * @brief Helper class for loading model files
     * 
     */
    class Model
    {
    public:
        struct Internal;
        std::unique_ptr<Internal> m_internal;
    private:
        void loadModel();
    protected:
    public:
        Model();
        Model(const AssetName &name);
        ~Model();

        Model(Model &o)             = delete;
        Model &operator=(Model &o)  = delete;
        Model(Model &&o)            = default;
        Model &operator=(Model &&o) = default;

        std::vector<Mesh> &getMeshes() const noexcept;
        std::vector<Texture> &getTextures() const noexcept;
    };
} // namespace core::assets

/** @} endgroup Assets */
