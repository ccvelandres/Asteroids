#include <assets/mesh.hpp>
#include <assets/loaders/obj.hpp>
#include <assets/asset-inventory.hpp>
#include <utils/logging.hpp>

#include <glm/gtx/string_cast.hpp>

namespace assets
{
    static void load_obj(const AssetName &name, Mesh::MeshData &meshData)
    {
        L_TAG("Mesh(Obj)");

        assets::loaders::obj::Loader objLoader;

        try
        {
            bool res = objLoader.loadFile(name);
            if (!res) L_THROW_RUNTIME("Failed to load obj: {}", name);

            for (auto &obj : objLoader.objects)
            {
                L_DEBUG("indicesCount: {}", obj.mesh.indices.size() / 3);

                std::unordered_map<Vertex, uint32_t> uniqueVertice;
                std::size_t                          duplicateVertice = 0;

                for (auto &index : obj.mesh.indices)
                {
                    /** Bounds check */
                    L_ASSERT(index.v + 3 < objLoader.vertices.size(), "Out of bonds index access for v");
                    L_ASSERT(index.vn + 3 < objLoader.normals.size(), "Out of bonds index access for vn");
                    L_ASSERT(index.vt + 3 < objLoader.texCoords.size(), "Out of bonds index access for vt");

                    /** Build vertex with vertices */

                    Vertex vertex = {
                        .v =
                            {
                                       objLoader.vertices[(3 * index.v) + 0],
                                       objLoader.vertices[(3 * index.v) + 1],
                                       objLoader.vertices[(3 * index.v) + 2],
                                       },
                        .vn =
                            {
                                       objLoader.normals[(3 * index.vn) + 0],
                                       objLoader.normals[(3 * index.vn) + 1],
                                       objLoader.normals[(3 * index.vn) + 2],
                                       },
                        .uv = {objLoader.texCoords[(2 * index.vt) + 0], objLoader.texCoords[(2 * index.vt) + 1]}
                    };

                    /** Maintain a map of with Vertex as keys and the
                     * indice as value to deduplicate vertexes
                     * if vertex already exists, dont push vertex then
                     * just add the index to indice vector
                     */
                    auto it = uniqueVertice.find(vertex);
                    if (it == uniqueVertice.end())
                    {
                        uint32_t index = static_cast<uint32_t>(meshData.vertices.size());
                        it             = uniqueVertice.insert(it, std::make_pair(vertex, index));
                        meshData.vertices.push_back(vertex);
                    }
                    else
                    {
                        duplicateVertice++;
                    }

                    meshData.indices.push_back(it->second);
                }

                L_DEBUG("VertexCount: {}", meshData.vertices.size());
                L_TRACE("Deduplicated {} vertice", duplicateVertice);
            }
        }
        catch (std::exception &ex)
        {
            L_THROW_RUNTIME("Failed to load obj: {}", name);
        }
    }

    static void load_defaults(const AssetName &name, Mesh::MeshData &meshData)
    {
        L_TAG("Mesh(Defaults)");

        try
        {
            /* code */
        }
        catch (const std::exception &e)
        {
            // std::cerr << e.what() << '\n';
        }
    }

    Mesh::MeshData::MeshData(const std::vector<Vertex> &vx, const std::vector<uint32_t> &idx) : vertices(vx), indices(idx) {}
    Mesh::MeshData::MeshData(std::vector<Vertex> &&vx, std::vector<uint32_t> &&idx) : vertices(vx), indices(idx) {}

    Mesh::Mesh() = default;
    Mesh::~Mesh()
    {
        L_TAG("~Mesh");
        L_TRACE("Internal resources freed ({})", static_cast<void *>(this));
    };

    Mesh::Mesh(const std::vector<Vertex> &vx, const std::vector<uint32_t> &idx) : m_meshData(vx, idx)
    {
        L_TAG("Mesh(&vertices, &indices)");
        L_TRACE("Internal resources initialized ({})", static_cast<void *>(this));
    }

    Mesh::Mesh(const AssetName &name)
    {
        L_TAG("Mesh(&filename)");

        // if(name.starts_with("defaults")){

        // }
        // else {
        //     if (name.ends_with(".obj")) {
        //         load_obj(name, this->m_meshData);
        //     }
        // }

        L_TRACE("Internal resources initialized ({})", static_cast<void *>(this));
    }

    const std::vector<Vertex>   &Mesh::getVertices() const noexcept { return m_meshData.vertices; }
    const std::vector<uint32_t> &Mesh::getIndices() const noexcept { return m_meshData.indices; }
} // namespace assets