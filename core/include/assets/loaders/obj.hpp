#pragma once

#include <string>

namespace assets::loaders
{
    class OBJLoader
    {
    public:
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

        struct Object {
            std::string name;
            Mesh mesh;
        };
    private:
        struct Face;

        std::vector<float> m_vertices;
        std::vector<float> m_normals;
        std::vector<float> m_texCoordinates;

        std::vector<Object> m_objects;

        // void pushObject(std::vector<Indices> &indices);
        void parse(std::istream &s);
    public:
        OBJLoader();

        bool loadFile(const std::string &filename);
    };
} // namespace assets::loaders