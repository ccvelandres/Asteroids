#pragma once

/**
 * @file core/assets/mesh-defaults.hpp
 * @author Cedric Velandres (ccvelandres@gmail.com)
 * @brief Private header containing mesh defaults
 * @addtogroup Assets
 * @{
 */

#include <unordered_map>
#include <core/assets/mesh.hpp>

namespace core::assets::defaults
{
    static const core::assets::Mesh mesh_quad =
        core::assets::Mesh("defaults/quad",
                           false,
                           false,
                           {
                               Vertex{.v          = {0.0f, 0.0f, 0.0f},
                                      .vn         = {0.0f, 0.0f, 1.0f},
                                      .uv         = {0.0f, 0.0f},
                                      .tangents   = {0.0f, 0.0f, 0.0f},
                                      .bitangents = {0.0f, 0.0f, 0.0f}},
                               Vertex{.v          = {0.0f, 1.0f, 0.0f},
                                      .vn         = {0.0f, 0.0f, 1.0f},
                                      .uv         = {0.0f, 1.0f},
                                      .tangents   = {0.0f, 0.0f, 0.0f},
                                      .bitangents = {0.0f, 0.0f, 0.0f}},
                               Vertex{.v          = {1.0f, 0.0f, 0.0f},
                                      .vn         = {0.0f, 0.0f, 1.0f},
                                      .uv         = {0.0f, 0.0f},
                                      .tangents   = {0.0f, 0.0f, 0.0f},
                                      .bitangents = {0.0f, 0.0f, 0.0f}},
                               Vertex{.v          = {1.0f, 1.0f, 0.0f},
                                      .vn         = {0.0f, 0.0f, 1.0f},
                                      .uv         = {0.0f, 1.0f},
                                      .tangents   = {0.0f, 0.0f, 0.0f},
                                      .bitangents = {0.0f, 0.0f, 0.0f}}
    },
                           {1, 0, 2, 1, 2, 3});

    static const std::unordered_map<std::string, const core::assets::Mesh *> mesh_defaults = {
        {"defaults/quad", &mesh_quad}
    };
} // namespace core::assets::defaults

/** @} endgroup Assets */
