
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

#include <utils/logging.hpp>
#include <utils/profiler.hpp>

#include <assets/loaders/obj.hpp>

namespace assets::loaders
{
    static bool fixIndex(int idx, int max, int *ret)
    {
        if (ret == NULL) return false; /** skip if ret is null */
        if (idx == 0) return false;    /** zero index is not allowed accdg to spec */

        if (idx > 0)
            *ret = idx - 1; /** convert to zero-index */
        else if (idx < 0)
            *ret = max + idx; /** relative to max */
        return true;
    }

    static bool parseFace(const char **token, int vsize, int vtsize, int vnsize, OBJLoader::Indices &indices)
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

    static std::size_t parseFloat(const char **token, float *f, std::size_t maxCount, float defaultValue = 1.0)
    {
        L_TAG("parseFloat");
        const char *str = *token;
        char       *p;
        std::size_t i          = 0;
        std::size_t matchCount = 0;
        for (f[i] = strtof(*token, &p); i++ < maxCount; f[i] = strtof(*token, &p))
        {
            if (*token == p)
                f[i] = defaultValue;
            else
                matchCount++;
            *token = p;
        }
        return matchCount;
    }

    struct OBJLoader::Face
    {
        std::vector<Indices> indices;
    };

    static bool pushObject(OBJLoader::Object               &obj,
                           std::vector<OBJLoader::Indices> &indices,
                           std::vector<float>               vertices,
                           std::string                     &name)
    {

        /** @todo: maybe add triangulation? */
        
    }

    void OBJLoader::parse(std::istream &s)
    {
        L_TAG("parseOBJ");

        OBJLoader::Indices   maxIndices;
        std::vector<Indices> indices;
        std::vector<float>   v;
        std::vector<float>   vt;
        std::vector<float>   vn;

        Object      object;
        std::string objectName;

        std::size_t unparsedLines = 0;

        while (s.peek() != EOF)
        {
            PROFILER_BLOCK("parseOBJ::innerLoop");

            std::string line;
            std::getline(s, line);

            /** Skip empty line */
            if (line.empty()) continue;

            /** Trim whitespace */
            if (line.starts_with(' ')) line.erase(0, line.find_first_not_of(' '));

            /** Skip comments */
            if (line.starts_with('#')) continue;

            if (line[0] == 'v')
            {
                if (line[1] == ' ')
                {
                    std::size_t index = v.size();
                    v.resize(v.size() + 3);

                    /** Parse geometry vertex directly to v */
                    const char *token = &line[2];
                    parseFloat(&token, &v[index], 3);

                    /** @todo: vertex color parsing? */
                }
                else if (line[1] == 'n')
                {
                    std::size_t index = vn.size();
                    vn.resize(vn.size() + 3);

                    /** Parse vertex normals directly to vn */
                    const char *token = &line[3];
                    parseFloat(&token, &vn[index], 3);
                }
                else if (line[1] == 't')
                {
                    std::size_t index = vt.size();
                    vt.resize(vt.size() + 2);

                    /** Parse texture coordinates directly to vt */
                    const char *token = &line[3];
                    parseFloat(&token, &vt[index], 2);
                }
            }
            else if (line[0] == 'f')
            {
                const char *token = &line[2];
                const char *str   = token;

                while (token[0] != '\n' || token[0] != '\r' || token[0] != '\0')
                {
                    OBJLoader::Indices ind(-1);

                    if (!parseFace(&token,
                                   static_cast<int>(v.size()),
                                   static_cast<int>(vt.size()),
                                   static_cast<int>(vn.size()),
                                   ind))
                    {
                        L_THROW_RUNTIME("Failed to parse: {} ", str);
                    }

                    maxIndices.v  = ind.v > maxIndices.v ? ind.v : maxIndices.v;
                    maxIndices.vt = ind.vt > maxIndices.vt ? ind.vt : maxIndices.vt;
                    maxIndices.vn = ind.vn > maxIndices.vn ? ind.vn : maxIndices.vn;

                    indices.push_back(ind);
                    token += std::strspn(token, " \t\r\n");
                }
            }
            else if (line[0] == 'o')
            {
                /** Push object if there is already indices */
                bool ret = pushObject(object, indices, v, objectName);
                if (ret)
                {
                    m_objects.push_back(object);
                }

                const char *token = &line[2];
                objectName        = std::string(token, std::strcspn(token, " \t\r"));
            }
            else
            {
                unparsedLines++;
            }
        }

        /** Check max indices if fits within vertices sizes */
        if (static_cast<int>(v.size() / 3) > maxIndices.v)
            L_WARN("Geometry indice out of bounds (index: {})", maxIndices.v);
        if (static_cast<int>(vt.size() / 3) > maxIndices.vt)
            L_WARN("Geometry indice out of bounds (index: {})", maxIndices.vt);
        if (static_cast<int>(vn.size() / 3) > maxIndices.vn)
            L_WARN("Geometry indice out of bounds (index: {})", maxIndices.vn);

        /** Move vectors to class */
        std::swap(v, m_vertices);
        std::swap(vt, m_texCoordinates);
        std::swap(vn, m_normals);
    }

    bool OBJLoader::loadFile(const std::string &filename)
    {
        L_TAG("OBJLoader::loadFile");

        if (!std::filesystem::exists(filename))
        {
            L_THROW_RUNTIME("Could not find file to load: {}", filename);
        }

        std::ifstream f(filename, std::ios::ate);
        std::size_t   fileSize = (std::size_t)f.tellg();
        f.seekg(0);

        parse(f);
    }

    OBJLoader::OBJLoader() { L_TAG("OBJLoader::OBJLoader"); }
} // namespace assets::loaders