#pragma once

#include <string>

namespace assets::loaders::obj
{
    struct Indices
    {
        int v;
        int vt;
        int vn;

        Indices() : v(-1), vt(-1), vn(-1) {}
        Indices(int idx) : v(idx), vt(idx), vn(idx) {}
        Indices(int vidx, int vtidx, int vnidx) : v(vidx), vt(vtidx), vn(vnidx) {}
    };

    struct Mesh
    {
        std::vector<Indices> indices;      /** indices for each face */
        std::vector<int>     verticeCount; /** vertice count per face */
    };

    struct Object
    {
        std::string name;
        Mesh        mesh;
    };

    class Loader
    {
    public:
    private:
        std::vector<float> m_vertices;
        std::vector<float> m_normals;
        std::vector<float> m_texCoordinates;

        std::vector<Object> m_objects;

        bool parse(std::istream &s) noexcept;
    public:
        Loader();

        bool loadFile(const std::string &filename);
    };
} // namespace assets::loaders::obj