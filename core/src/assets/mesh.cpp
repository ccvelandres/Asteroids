#include <assets/mesh.hpp>
#include <assets/loaders/obj.hpp>
#include <utils/logging.hpp>

#include <glm/gtx/string_cast.hpp>

#include <unordered_map>

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
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;

        try
        {
            bool res = objLoader.loadFile(filename);
            if (!res) L_THROW_RUNTIME("Failed to load obj: {}", filename);

            for (auto &obj : objLoader.objects)
            {
                L_DEBUG("indicesCount: {}", obj.mesh.indices.size() / 3);

                std::unordered_map<Vertex, uint32_t> uniqueVertice;
                std::size_t duplicateVertice = 0;

                for (auto &index : obj.mesh.indices)
                {
                    /** Bounds check */
                    L_ASSERT(index.v + 3 < objLoader.vertices.size(), "Out of bonds index access for v");
                    L_ASSERT(index.vn + 3 < objLoader.normals.size(), "Out of bonds index access for vn");
                    L_ASSERT(index.vt + 3 < objLoader.texCoords.size(), "Out of bonds index access for vt");

                    /** Build vertex with vertices */

                    Vertex vertex = {
                        .position = {
                            objLoader.vertices[(3 * index.v) + 0],
                            objLoader.vertices[(3 * index.v) + 1],
                            objLoader.vertices[(3 * index.v) + 2],
                        },
                        .normals = {
                            objLoader.normals[(3 * index.vn) + 0],
                            objLoader.normals[(3 * index.vn) + 1],
                            objLoader.normals[(3 * index.vn) + 2],
                        },
                        .texCoords = {
                            objLoader.texCoords[(2 * index.vt) + 0],
                            objLoader.texCoords[(2 * index.vt) + 1] 
                        }
                    };


                    /** Maintain a map of with Vertex as keys and the
                     * indice as value to deduplicate vertexes
                     * if vertex already exists, dont push vertex then
                     * just add the index to indice vector
                     */
                    auto it = uniqueVertice.find(vertex);
                    if(it == uniqueVertice.end())
                    {
                        uint32_t index = static_cast<uint32_t>(vertices.size());
                        it = uniqueVertice.insert(it, std::make_pair(vertex, index));
                        vertices.push_back(vertex);
                    }
                    else {
                        duplicateVertice++;
                    }
                    
                    indices.push_back(it->second);
                }

                L_DEBUG("VertexCount: {}", vertices.size());
                L_TRACE("Deduplicated {} vertice", duplicateVertice);
            }

            // m_internal = std::make_unique<Internal>();
        }
        catch (std::exception &ex)
        {
            L_THROW_RUNTIME("Failed to load obj: {}", filename);
        }

        m_internal = std::make_unique<Internal>(std::move(vertices), std::move(indices));
    }

    Mesh::Mesh()                    = default;
    Mesh::Mesh(Mesh &&o)            = default;
    Mesh &Mesh::operator=(Mesh &&o) = default;
    Mesh::~Mesh()                   = default;

    const std::vector<Vertex>   &Mesh::getVertices() const { return m_internal->vertices; }
    const std::vector<uint32_t> &Mesh::getIndices() const { return m_internal->indices; }
} // namespace assets