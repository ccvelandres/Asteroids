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

namespace assets
{
    /**
     * @brief Helper class for parsing mesh files
     *
     */
    class Mesh
    {
    public:
        struct MeshData
        {
            std::vector<Vertex>   vertices;
            std::vector<uint32_t> indices;

            MeshData()  = default;
            ~MeshData() = default;
            MeshData(const std::vector<Vertex> &vx, const std::vector<uint32_t> &idx);
            MeshData(std::vector<Vertex> &&vx, std::vector<uint32_t> &&idx);
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
} // namespace assets

/** @} endgroup Assets */