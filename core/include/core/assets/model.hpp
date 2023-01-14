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

        std::vector<Mesh> m_vMesh;
        std::vector<Texture> m_vTexture;
    private:
    protected:
    public:
        Model();
        Model(const AssetName &name);
        ~Model();

        Model(Model &o)             = delete;
        Model &operator=(Model &o)  = delete;
        Model(Model &&o)            = default;
        Model &operator=(Model &&o) = default;
    };
} // namespace core::assets

/** @} endgroup Assets */
