#include <assets/mesh.hpp>

#include <utils/logging.hpp>

#include <assets/loaders/obj.hpp>

namespace assets
{
    struct Mesh::Internal
    {
        const std::vector<Vertex>   vertices;
        const std::vector<uint32_t> indices;

        Internal(const std::vector<Vertex> &vertices, const std::vector<uint32_t> &indices)
            : vertices(vertices),
              indices(indices)
        {
            L_TAG("Mesh::Internal");
            /** Check if vertices count is equal to indices count*/
            if (vertices.size() != indices.size()) L_THROW_RUNTIME("Vertice count is not equal to indice count");
            L_TRACE("Internal resources initialized ({})", static_cast<void *>(this));
        }

        ~Internal()
        {
            L_TAG("Mesh::~Internal");
            L_TRACE("Internal resources freed ({})", static_cast<void *>(this));
        }
    };

    Mesh::Mesh(const std::vector<Vertex> &vertices, const std::vector<uint32_t> &indices)
        : m_internal(std::make_unique<Internal>(vertices, indices))
    {
    }

    Mesh::Mesh(const std::string &filename)
    {
        L_TAG("Mesh::Mesh(std::string&)");
        assets::loaders::obj::Loader objLoader;

        try
        {
            bool res = objLoader.loadFile(filename);
            if (!res) L_THROW_RUNTIME("Failed to load obj: {}", filename);

            for (auto &obj : objLoader.objects)
            {
                for (auto &index : obj.mesh.indices)
                {
                }
            }

            // m_internal = std::make_unique<Internal>();
        }
        catch (std::exception &ex)
        {
            L_THROW_RUNTIME("Failed to load obj: {}", filename);
        }
    }

    Mesh::Mesh(Mesh &&o)            = default;
    Mesh &Mesh::operator=(Mesh &&o) = default;
    Mesh::~Mesh()                   = default;

    const std::vector<Vertex>   &Mesh::getVertices() const { return m_internal->vertices; }
    const std::vector<uint32_t> &Mesh::getIndices() const { return m_internal->indices; }
} // namespace assets