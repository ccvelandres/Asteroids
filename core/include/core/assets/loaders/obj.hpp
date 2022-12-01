#pragma once

#include <string>

namespace core::assets::loaders::obj
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

        bool parse(std::istream &s) noexcept;
    public:
        std::vector<float> vertices; /** Geometry vertices */
        std::vector<float> texCoords; /** Texture coordinates */
        std::vector<float> normals; /** Vertex normals */
        std::vector<Object> objects; /** all objects in obj file */
        
        Loader();
        bool loadFile(const std::string &filename);
    };
} // namespace core::assets::loaders::obj