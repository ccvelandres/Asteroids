#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>

struct Vertex
{
    glm::vec3 position; /** Geometry vertex */
    glm::vec3 normals; /** Vertex normals */
    glm::vec2 texCoords; /** Texture coordinates */

    bool operator==(const Vertex &o) const
    {
        return this->position == o.position && this->normals == o.normals && this->texCoords == o.texCoords;
    }
};

namespace std
{
    /** Create hash function for vertex */
    template <>
    struct hash<Vertex>
    {
        size_t operator()(const Vertex &o) const
        {
            std::size_t h_pos = hash<glm::vec3>()(o.position);
            std::size_t h_nor = hash<glm::vec3>()(o.normals);
            std::size_t h_tex = hash<glm::vec2>()(o.texCoords);
            return ((h_pos ^ (h_nor << 1) >> 1) ^ (h_tex << 1) >> 1);
        }
    };
}