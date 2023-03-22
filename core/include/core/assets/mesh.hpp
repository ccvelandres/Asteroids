#pragma once

/**
 * @file core/assets/mesh.hpp
 * @author Cedric Velandres (ccvelandres@gmail.com)
 *
 * @addtogroup Assets
 * @{
 */

#include <memory>
#include <cstdint>
#include <vector>

#include "asset-inventory.hpp"
#include "../graphics/vertex.hpp"

namespace core::assets
{
    /** Forward declaration for Model */
    class Model;

    /**
     * @brief Helper class for parsing mesh files
     *
     */
    class Mesh
    {
    public:
    private:
        std::string           m_name;
        bool                  m_hasTangents;   /** Whether mesh data has tangents */
        bool                  m_hasBitangents; /** Whether mesh data has bitangents */
        std::vector<Vertex>   m_vertices;
        std::vector<uint32_t> m_indices;
    public:
        Mesh(const AssetName &name);
        Mesh(const std::string    &name,
             bool                  hasTangents,
             bool                  hasBitangents,
             std::vector<Vertex>   vertices,
             std::vector<uint32_t> indices);
        Mesh()  = default;
        ~Mesh() = default;

        Mesh(const Mesh &)            = default;
        Mesh &operator=(const Mesh &) = default;
        Mesh(Mesh &&o)                = default;
        Mesh &operator=(Mesh &&o)     = default;

        /** @brief Returns the mesh name */
        const std::string &name() const noexcept;
        /** @brief Returns if mesh has tangents */
        bool hasTangents() const noexcept;
        /** @brief Returns if mesh has bitangents */
        bool hasBitangents() const noexcept;
        /** @brief Returns reference to vector containing the vertices */
        const std::vector<Vertex> &getVertices() const noexcept;
        /** @brief Returns reference to vector containing the indices */
        const std::vector<uint32_t> &getIndices() const noexcept;

        friend Model;
    };
} // namespace core::assets

/** @} endgroup Assets */