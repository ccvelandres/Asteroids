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
    /** @todo: re-enable const defaults for some mesh types */
    // inline static const Mesh xy_quad = {
    //     .m_name = "defaults/xy_quad",
    //     .m_hasTangents = false,
    //     .m_hasBitangets = false,
    //     .m_vertices = {
    //         {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}},
    //         {{0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}},
    //         {{1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}},
    //         {{1.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}}
    //     },
    //     .m_indices = {
    //         1, 0, 2, 1, 2, 3
    //     }
    // };

    // const std::unordered_map<std::string, const Mesh::MeshData *> mesh_defaults = {
    //     {"quad", &core::assets::defaults::mesh::xy_quad}
    // };
} // namespace core::assets::defaults::mesh

/** @} endgroup Assets */
