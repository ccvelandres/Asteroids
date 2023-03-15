#include <assets/mesh.hpp>
#include <assets/mesh-defaults.hpp>
#include <assets/loaders/obj.hpp>
#include <assets/asset-inventory.hpp>
#include <utils/logging.hpp>

#include <glm/gtx/string_cast.hpp>

namespace core::assets
{
    Mesh::Mesh(const std::string    &name,
               bool                  hasTangents,
               bool                  hasBitangents,
               std::vector<Vertex>   vertices,
               std::vector<uint32_t> indices)
        : m_name(name),
          m_hasTangents(hasTangents),
          m_hasBitangents(hasBitangents),
          m_vertices(vertices),
          m_indices(indices)
    {
    }

    Mesh::Mesh(const AssetName &name)
    {
        L_TAG("Mesh::Mesh");
        auto it = defaults::mesh_defaults.find(name);
        if (it != defaults::mesh_defaults.end())
        {
            this->m_name          = it->second->m_name;
            this->m_hasTangents   = it->second->m_hasTangents;
            this->m_hasBitangents = it->second->m_hasBitangents;
            this->m_vertices      = it->second->m_vertices;
            this->m_indices       = it->second->m_indices;
        }

        L_THROW_RUNTIME("Mesh(AssetName) only supports defaults");
    }

    const std::string &Mesh::name() const noexcept { return this->m_name; }
    bool               Mesh::hasTangents() const noexcept { return this->m_hasTangents; }
    bool               Mesh::hasBitangents() const noexcept { return this->m_hasBitangents; }
    const std::vector<Vertex>   &Mesh::getVertices() const noexcept { return m_vertices; }
    const std::vector<uint32_t> &Mesh::getIndices() const noexcept { return m_indices; }

} // namespace core::assets