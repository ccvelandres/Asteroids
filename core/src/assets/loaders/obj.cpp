
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include <array>

#include <utils/logging.hpp>
#include <utils/profiler.hpp>

#include <assets/loaders/obj.hpp>

namespace core::assets::loaders::obj
{
    /********************************************************************************
     * Private structures
     *******************************************************************************/
    struct Face
    {
        std::vector<Indices> indices;
    };

    struct ProcessGroup
    {
        std::vector<Face> faces;
    };

    static bool fixIndex(int idx, int max, int *ret) noexcept
    {
        if (ret == NULL) return false; /** skip if ret is null */
        if (idx == 0) return false;    /** zero index is not allowed accdg to spec */

        if (idx > 0)
            *ret = idx - 1; /** convert to zero-index */
        else if (idx < 0)
            *ret = max + idx; /** relative to max */
        return true;
    }

    static bool parseFace(const char **token, int vsize, int vtsize, int vnsize, Indices &indices) noexcept
    {
        L_TAG("parseFace");
        char *p;
        int   idx;

        /** Get vertex index */
        if (!fixIndex(std::atoi(*token), vsize, &indices.v)) return false;

        (*token) += std::strcspn(*token, "/ \r\n");
        if ((*token)[0] != '/') return true; /** Parse end if no more / */

        if ((*token)[1] == '/')
        {
            /** i//k */
            (*token) += 2;
            if (!fixIndex(std::atoi(*token), vnsize, &indices.vn)) return false;
            (*token) += std::strcspn(*token, "/ \r\n");
            if ((*token)[0] != '/') return true; /** Parse end if no more / */
        }
        else
        {
            /** i/j */
            (*token)++;
            if (!fixIndex(std::atoi(*token), vtsize, &indices.vt)) return false;
            (*token) += std::strcspn(*token, "/ \r\n");
            if ((*token)[0] != '/') return true; /** Parse end if no more / */
        }

        /** i/j/k */
        if ((*token)[1] != '/')
        {
            (*token)++;
            if (!fixIndex(std::atoi(*token), vnsize, &indices.vn)) return false;
        }

        (*token) += std::strcspn(*token, "/ \r\n");
        return true;
    }

    template <std::size_t maxCount>
    static std::size_t parseFloat(const char *const *token, std::array<float, maxCount> &f, float defaultValue = 1.0) noexcept
    {
        L_TAG("parseFloat");
        const char *str = *token;
        char       *p;
        std::size_t i          = 0;
        std::size_t matchCount = 0;

        do
        {
            f[i] = std::strtof(str, &p);
            if (str == p) return matchCount;
            matchCount++;
            str = p;
        } while (++i < maxCount);

        return matchCount;
    }

    static bool triangulateFaces(ProcessGroup &processGroup) noexcept
    {
        /** @todo: maybe add triangulation? */
        return true;
    }

    static bool pushObject(Object             &obj,
                           ProcessGroup       &processGroup,
                           std::vector<float> &vertices,
                           std::string        &name,
                           bool                triangulate = false) noexcept
    {
        if (processGroup.faces.empty()) return false;

        /** Process faces for the mesh */
        if (!processGroup.faces.empty())
        {
            bool ret;

            for (std::size_t i = 0; i < processGroup.faces.size(); i++)
            {
                const auto &face = processGroup.faces[i];

                if (triangulate && triangulateFaces(processGroup))
                {
                    /** @todo: add triangulation for faces */
                }
                else
                {
                    /** move indices to mesh object */
                    for (const auto &indice : face.indices)
                    {
                        obj.mesh.indices.push_back(indice);
                    }

                    obj.mesh.verticeCount.push_back(face.indices.size());
                }
            }
        }

        obj.name = name;
        processGroup.faces.clear();

        return true;
    }

    bool Loader::parse(std::istream &s) noexcept
    {
        L_TAG("Loader::parse");
        bool ret;

        std::vector<float> v;
        std::vector<float> vt;
        std::vector<float> vn;

        Object       object;
        Indices      maxIndices;
        ProcessGroup processGroup;
        std::string  objectName = "test";

        std::vector<std::string> unparsedLines;

        while (s.peek() != EOF)
        {
            PROFILER_BLOCK("Loader::parse::innerLoop");

            std::string line;
            std::getline(s, line);

            /** Skip empty line */
            if (line.empty()) continue;

            /** Trim whitespace */
            if (line[0] == ' ') line.erase(0, line.find_first_not_of(' '));

            /** Skip comments */
            if (line[0] == '#') continue;

            if (line[0] == 'v')
            {
                if (line[1] == ' ')
                {
                    std::array<float, 3> tmp;

                    /** Parse geometry vertex */
                    const char *token = &line[2];
                    if (parseFloat(&token, tmp) == 3)
                        v.insert(v.end(), tmp.begin(), tmp.end());
                    else
                    {
                        L_WARN("Failed to parse: {}", line);
                        return false;
                    }

                    /** @todo: vertex color parsing? */
                }
                else if (line[1] == 'n')
                {
                    std::array<float, 3> tmp;

                    /** Parse vertex normals */
                    const char *token = &line[3];
                    if (parseFloat(&token, tmp) == 3)
                        vn.insert(vn.end(), tmp.begin(), tmp.end());
                    else
                    {
                        L_WARN("Failed to parse: {}", line);
                        return false;
                    }
                }
                else if (line[1] == 't')
                {
                    std::array<float, 2> tmp;

                    /** Parse texture coordinates */
                    const char *token = &line[3];
                    if (parseFloat(&token, tmp) == 2)
                        vt.insert(vt.end(), tmp.begin(), tmp.end());
                    else
                    {
                        L_WARN("Failed to parse: {}", line);
                        return false;
                    }
                }
            }
            else if (line[0] == 'f')
            {
                const char *token = &line[2];
                const char *str   = token;
                Face        face;

                while (token[0] != '\0' && token[0] != '\n' && token[0] != '\r')
                {
                    Indices ind(-1);

                    if (!parseFace(&token,
                                   static_cast<int>(v.size()),
                                   static_cast<int>(vt.size()),
                                   static_cast<int>(vn.size()),
                                   ind))
                    {
                        L_WARN("Failed to parse: {}", str);
                        return false;
                    }

                    maxIndices.v  = ind.v > maxIndices.v ? ind.v : maxIndices.v;
                    maxIndices.vt = ind.vt > maxIndices.vt ? ind.vt : maxIndices.vt;
                    maxIndices.vn = ind.vn > maxIndices.vn ? ind.vn : maxIndices.vn;

                    face.indices.push_back((ind));
                    token += std::strspn(token, " \t\r\n");
                }

                processGroup.faces.push_back((face));
            }
            else if (line[0] == 'o')
            {
                /** Push object if there is already indices */
                ret = pushObject(object, processGroup, v, objectName);
                if (ret)
                {
                    L_DEBUG("Parsed mesh with {} faces", object.mesh.verticeCount.size());
                    objects.push_back(object);
                }

                /** Start a new object/group */
                object = Object();

                const char *token = &line[2];
                objectName        = std::string(token, std::strcspn(token, " \t\r"));
            }
            else
            {
                unparsedLines.push_back(line);
            }
        }

        L_DEBUG("Parsing done.");
        L_DEBUG("      Found {} geometry vertices", v.size() / 3);
        L_DEBUG("      Found {} vertex normals", vn.size() / 3);
        L_DEBUG("      Found {} texture coordinates", vt.size() / 2);
        L_DEBUG("            {} lines unprocessed", unparsedLines.size());
        for (std::size_t linenum = 0; linenum < unparsedLines.size(); linenum++)
            L_TRACE("Unparsed {}: {}", linenum, unparsedLines[linenum]);

        /** Check max indices if fits within vertices sizes */
        if (static_cast<int>(v.size() / 3) <= maxIndices.v)
            L_WARN("Geometry indice out of bounds (index: {})", maxIndices.v);
        if (static_cast<int>(vt.size() / 2) <= maxIndices.vt)
            L_WARN("Texture indice out of bounds  (index: {})", maxIndices.vt);
        if (static_cast<int>(vn.size() / 3) <= maxIndices.vn)
            L_WARN("Normal indice out of bounds   (index: {})", maxIndices.vn);

        /** Export last group */
        ret = pushObject(object, processGroup, v, objectName);
        if (ret)
        {
            objects.push_back((object));
        }

        /** Move vectors to class */
        std::swap(v, vertices);
        std::swap(vt, texCoords);
        std::swap(vn, normals);

        return true;
    }

    bool Loader::loadFile(const std::string &filename)
    {
        bool ret = false;
        L_TAG("Loader::loadFile");

        if (!std::filesystem::exists(filename))
        {
            L_THROW_RUNTIME("Could not find file to load: {}", filename);
        }

        std::ifstream f(filename, std::ios::ate);
        std::size_t   fileSize = (std::size_t)f.tellg();
        f.seekg(0);

        normals.clear();
        objects.clear();
        texCoords.clear();
        vertices.clear();

        L_DEBUG("Parsing {}", filename);
        try
        {
            ret = parse(f);
        }
        catch (const std::exception &e)
        {
            L_ERROR("Exeption during parsing file: {} ({})", filename, e.what());
        }

        return ret;
    }

    Loader::Loader() { L_TAG("Loader::Loader"); }
} // namespace core::assets::loaders::obj