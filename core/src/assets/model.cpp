#include <core/assets/model.hpp>
#include <core/utils/logging.hpp>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <glm/glm.hpp>

namespace core::assets
{

    static void loadMaterial(const aiScene *scene, unsigned int index)
    {
        L_TAG("Model::loadMaterial");

        L_ASSERT(scene->mNumMaterials > index, "index exceeds mNumMaterials");

        std::vector<Mesh::Texture_> textures;

        auto &material = scene->mMaterials[index];

        // load diffuse textures
        for (int i = 0; i < material->GetTextureCount(aiTextureType_DIFFUSE); ++i)
        {
            Mesh::Texture_ texture;
            aiString       texturePath;
            auto           t = material->GetTexture(aiTextureType_DIFFUSE, i, &texturePath);

            texture.name = texturePath.C_Str();
            textures.push_back(texture);
            L_TRACE("Diffuse: {}", texture.name);
        }
    }

    static Mesh &&loadMesh(const aiScene *scene, const aiMesh *mesh)
    {
        L_TAG("Model::loadMesh");

        std::vector<Mesh::Vertex_>  vertices;
        std::vector<unsigned int>   indices;
        std::vector<Mesh::Texture_> textures;

        L_TRACE("mName: {}", mesh->mName.C_Str());

        // Reserve memory for vectors
        vertices.reserve(mesh->mNumVertices);
        indices.reserve(mesh->mNumFaces);

        // Do a somple check that there is texture coordinates
        if ( NULL == mesh->mTextureCoords[0])
        {
            L_WARN("Mesh has no texture coordinates");
        }

        // Load mesh vertices
        for (int i = 0; i < mesh->mNumVertices; ++i)
        {
            Mesh::Vertex_ vertex;
            // geometry vertex
            {
                auto &v  = mesh->mVertices[i];
                vertex.v = glm::vec3(v.x, v.y, v.z);
            }
            // vertex normals
            if (mesh->HasNormals())
            {
                auto &vn  = mesh->mNormals[i];
                vertex.vn = glm::vec3(vn.x, vn.y, vn.z);
            }
            // texture coordinates
            /** @todo: currently assumes models have only one tex coordinates */
            if (mesh->mTextureCoords[0])
            {
                // texture coordinates
                auto &uv  = mesh->mTextureCoords[0][i];
                vertex.uv = glm::vec2(uv.x, uv.y);

                // tangent vectors
                auto &tangent   = mesh->mTangents[i];
                vertex.tangents = glm::vec3(tangent.x, tangent.y, tangent.z);

                // bitangent vectors
                auto &bitangent   = mesh->mBitangents[i];
                vertex.bitangents = glm::vec3(bitangent.x, bitangent.y, bitangent.z);
            }
            else
            {
                // fallback texcoordinates
                vertex.uv = glm::vec2(0.0f, 0.0f);
            }

            vertices.push_back(vertex);
        }

        // load vertice indices
        for (int i = 0; i < mesh->mNumFaces; ++i)
        {
            /** each face has n indices which corresponds to index of vertices that
             * make up the face
             * ie. if a face has 3 vertices, then a face has 3 indices that matches
             * the indices of the vertices from the vertex buffer
             */
            auto &face = mesh->mFaces[i];
            for (int j = 0; j < face.mNumIndices; ++j)
            {
                indices.push_back(face.mIndices[j]);
            }
        }

        // load mesh materials

        core::assets::loadMaterial(scene, mesh->mMaterialIndex);
    }

    static void loadNode(const aiScene *scene, aiNode *node)
    {
        L_TAG("Model::loadNode");

        L_TRACE("mName: {}", node->mName.C_Str());
        L_TRACE("mNumMeshes: {}", node->mNumMeshes);
        L_TRACE("mNumChildren: {}", node->mNumChildren);

        // load mesh data
        for (int i = 0; i < node->mNumMeshes; ++i)
        {
            aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
            core::assets::loadMesh(scene, mesh);
        }

        for (int i = 0; i < node->mNumChildren; ++i)
        {
            core::assets::loadNode(scene, node->mChildren[i]);
        }
    }

    Model::Model(const AssetName &name)
    {
        L_TAG("Model::Model");

        L_DEBUG("Loading model: {}", name);

        {
            Assimp::Importer importer;
            const aiScene   *scene = importer.ReadFile(name,
                                                     aiProcess_Triangulate | aiProcess_GenNormals
                                                         | aiProcess_CalcTangentSpace | aiProcess_JoinIdenticalVertices);

            if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
            {
                L_THROW_RUNTIME("Error during model import: {}", importer.GetErrorString());
            }

            core::assets::loadNode(scene, scene->mRootNode);
        }
    }

    Model::~Model() = default;
} // namespace core::assets
