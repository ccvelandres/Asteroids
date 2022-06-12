#pragma once

#include <memory>
#include <cstdint>
#include <vector>

#include <renderer/vertex.hpp>

namespace assets
{
    class Mesh
    {
        struct Internal;
        std::unique_ptr<Internal> m_internal;
    public:
        Mesh( const std::vector<Vertex> &vertices, const std::vector<uint32_t> indices );
        Mesh( Mesh &&o );
        Mesh &operator=( Mesh &&o );
        ~Mesh();

        const std::vector<Vertex>   &getVertices() const;
        const std::vector<uint32_t> &getIndices() const;
    };
} // namespace assets