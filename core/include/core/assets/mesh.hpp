#pragma once

/**
 * @file core/assets/mesh.cpp
 * @author Cedric Velandres (ccvelandres@gmail.com)
 *
 * @addtogroup Assets
 * @{
 */

#include <memory>
#include <cstdint>
#include <vector>

#include "asset-inventory.hpp"
#include "../graphics/renderer/vertex.hpp"

namespace core::assets
{
    /**
     * @brief Helper class for parsing mesh files
     *
     */
    class Mesh
    {
    public:
        /**
         * @brief Container for vertices used for 3D mesh
         *
         */
        struct Vertex_
        {
            glm::vec3 v;  /** Geometry vertex */
            glm::vec3 vn; /** Vertex normals */
            glm::vec2 uv; /** Texture coordinates */
            glm::vec3 tangents; /** tangets */
            glm::vec3 bitangents; /** bitangents */
        };

        struct Texture_
        {
            std::string name;
        };

        struct MeshData
        {
            std::vector<Vertex>   vertices;
            std::vector<uint32_t> indices;

            MeshData &operator=(MeshData &&)      = default;
            MeshData &operator=(const MeshData &) = default;
        };

    private:
        MeshData m_meshData;

    public:

        Mesh();
        /**
         * @brief Construct a new Mesh object
         *
         * @param name name of mesh asset
         */
        Mesh(const AssetName &name);
        Mesh(const std::vector<Vertex> &vertices, const std::vector<uint32_t> &indices);
        ~Mesh();

        Mesh(const Mesh &)            = delete;
        Mesh &operator=(const Mesh &) = delete;
        Mesh(Mesh &&o)                = default;
        Mesh &operator=(Mesh &&o)     = default;

        /**
         * @brief Get the vector containing all Vertex objects for the mesh
         *
         * @return const std::vector<Vertex>& reference to vector of Vertex
         */
        const std::vector<Vertex> &getVertices() const noexcept;
        /**
         * @brief Get the vector containing all indices for the mesh
         *
         * @return const std::vector<uint32_t>& reference to vector of indices
         */
        const std::vector<uint32_t> &getIndices() const noexcept;
    };
} // namespace core::assets

/** @} endgroup Assets */