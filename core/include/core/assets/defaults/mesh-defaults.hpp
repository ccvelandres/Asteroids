#pragma once

/**
 * @file assets/defaults/mesh-defaults.hpp
 * @author Cedric Velandres (ccvelandres@gmail.com)
 * @todo: maybe replace this is runtime generation?
 *
 * @addtogroup Assets
 * @{
 */

#include <vector>
#include <string>
#include <unordered_map>

#include "../mesh.hpp"

namespace core::assets::defaults::mesh
{

    inline static const Mesh::MeshData xy_quad = {
        .vertices = {
            {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
            {{0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
            {{1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},
            {{1.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}}
        },
        .indices = {
            1, 0, 2, 1, 2, 3
        }
    };

    const std::unordered_map<std::string, const Mesh::MeshData *> mesh_defaults = {
        {"quad", &core::assets::defaults::mesh::xy_quad}
    };
} // namespace core::assets::defaults::mesh

/** @} endgroup Assets */
