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
#include <core/utils/hash.hpp>

/**
 * @brief Container for vertices used for 3D mesh
 *
 */
struct Vertex
{
    glm::vec3 v{0.0f, 0.0f, 0.0f};          /** Geometry vertex */
    glm::vec3 vn{0.0f, 0.0f, 0.0f};         /** Vertex normals */
    glm::vec2 uv{0.0f, 0.0f};               /** Texture coordinates */
    glm::vec3 tangents{0.0f, 0.0f, 0.0f};   /** Vertex tangents */
    glm::vec3 bitangents{0.0f, 0.0f, 0.0f}; /** Vertex bitangents */

    bool operator==(const Vertex &o) const
    {
        return this->v == o.v && this->vn == o.vn && this->uv == o.uv
            && this->tangents == o.tangents && this->bitangents == o.bitangents;
    }
};

namespace std
{
    /** Create hash function for vertex */
    template <>
    struct hash<Vertex>
    {
        size_t operator()(const Vertex &o) const
        {
            std::size_t hash = 0;
            core::utils::hash_combine(hash, o.v, o.vn, o.uv, o.tangents, o.bitangents);
            return hash;
        }
    };
} // namespace std

/** @} endgroup Renderer */
