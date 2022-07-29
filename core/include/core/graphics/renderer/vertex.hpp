#pragma once

/**
 * @file core/renderer/vertex.hpp
 * @author Cedric Velandres (ccvelandres@gmail.com)
 *
 * @addtogroup Renderer
 * @{
 */

#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>

/**
 * @brief Container for vertices used for 3D mesh
 *
 */
struct Vertex
{
    glm::vec3 v;  /** Geometry vertex */
    glm::vec3 vn; /** Vertex normals */
    glm::vec2 uv; /** Texture coordinates */

    bool operator==(const Vertex &o) const { return this->v == o.v && this->vn == o.vn && this->uv == o.uv; }
};

namespace std
{
    /** Create hash function for vertex */
    template <>
    struct hash<Vertex>
    {
        size_t operator()(const Vertex &o) const
        {
            std::size_t h_pos = hash<glm::vec3>()(o.v);
            std::size_t h_nor = hash<glm::vec3>()(o.vn);
            std::size_t h_tex = hash<glm::vec2>()(o.uv);
            return ((h_pos ^ (h_nor << 1) >> 1) ^ (h_tex << 1) >> 1);
        }
    };
} // namespace std

/** @} endgroup Renderer */
