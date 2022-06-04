#pragma once

#include "vertex.hpp"
#include <cstdint>

class Mesh
{
    const std::vector<Vertex>   m_vertices;
    const std::vector<uint32_t> m_indices;
public:
    Mesh( const std::vector<Vertex> &vertices, const std::vector<uint32_t> indices );

    const std::vector<Vertex>   &getVertices() const;
    const std::vector<uint32_t> &getIndices() const;
};
