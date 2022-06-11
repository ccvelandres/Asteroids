#include <assets/mesh.hpp>

#include <utils/logging.hpp>

namespace assets
{
    Mesh::Mesh( const std::vector<Vertex> &vertices, const std::vector<uint32_t> indices )
        : m_vertices( vertices ),
        m_indices( indices )
    {
        L_TAG( "Mesh::Mesh" );
        /** Check if vertices count is equal to indices count*/
        if ( vertices.size() != indices.size() ) L_THROW_RUNTIME( "Vertice count is not equal to indice count" );
    }

    const std::vector<Vertex>   &Mesh::getVertices() const { return m_vertices; }
    const std::vector<uint32_t> &Mesh::getIndices() const { return m_indices; }
}