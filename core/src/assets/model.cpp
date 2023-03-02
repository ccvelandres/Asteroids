#include <core/assets/model.hpp>
#include <core/utils/logging.hpp>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <glm/glm.hpp>

namespace core::assets
{

    struct Model::Internal
    {
        /** @todo: add parent-child relation for nodes */
        std::vector<Mesh>    m_meshes;
        std::vector<Texture> m_textures;
        const aiScene       *m_scene{nullptr};

        void loadModel();
    private:
        void processNode(aiNode *node);
        void loadMesh(int meshIndex);
        void loadMaterial(aiTextureType type, int materialIndex);
    };

    void Model::Internal::loadModel()
    {
        L_TAG("Model::Internal::loadModel");

        L_ASSERT(m_scene != nullptr, "aiScene does not have valid data");

        // Reserve vectors
        m_meshes.reserve(m_scene->mNumMeshes);
        m_textures.reserve(m_scene->mNumTextures);

        // Process root node
        processNode(m_scene->mRootNode);
    }

    void Model::Internal::processNode(aiNode *node)
    {
        L_TAG("Model::Model");
        L_ASSERT(m_scene != nullptr, "aiScene does not have valid data");

        // load node's meshes
        for (unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            loadMesh(node->mMeshes[i]);
        }

        // load materials
        for (int i = 0; i < m_scene->mNumMaterials; ++i)
        {
            // load textures
        }

        // load childrens
        for (int i = 0; i < node->mNumChildren; ++i)
            processNode(node->mChildren[i]);
    }

    void Model::Internal::loadMesh(int meshIndex)
    {
        L_TAG("Model::Internal::loadMesh");

        L_ASSERT(m_scene->mNumMeshes > meshIndex, "meshIndex exceeds numMesh in scene");
        const auto &mesh = m_scene->mMeshes[meshIndex];

        Mesh meshObject;
        meshObject.m_name          = std::string(mesh->mName.C_Str());
        meshObject.m_hasTangents   = mesh->HasTangentsAndBitangents();
        meshObject.m_hasBitangents = mesh->HasTangentsAndBitangents();

        auto &vertices = meshObject.m_vertices;
        auto &indices  = meshObject.m_indices;

        // Reserve memory for vectors
        vertices.reserve(mesh->mNumVertices);
        indices.reserve(mesh->mNumFaces);

        // Do a somple check that there is texture coordinates
        if (!mesh->HasTextureCoords(0)) L_WARN("Mesh has no texture coordinates");

        // load mesh vertices
        for (int i = 0; i < mesh->mNumVertices; ++i)
        {
            Vertex vertex;
            // geometry vertex
            auto &v  = mesh->mVertices[i];
            vertex.v = glm::vec3(v.x, v.y, v.z);

            // vertex normals
            if (mesh->HasNormals())
            {
                auto &vn  = mesh->mNormals[i];
                vertex.vn = glm::vec3(vn.x, vn.y, vn.z);
            }

            // texture coordinates
            /** @todo: support importing more than 1 uv channels */
            if (mesh->HasTextureCoords(0))
            {
                // texture coordinates
                auto &uv  = mesh->mTextureCoords[0][i];
                vertex.uv = glm::vec2(uv.x, uv.y);
            }
            else
            {
                // fallback texcoordinates
                vertex.uv = glm::vec2(0.0f, 0.0f);
            }

            if (mesh->HasTangentsAndBitangents())
            {
                // tangent vectors
                auto &tangent   = mesh->mTangents[i];
                vertex.tangents = glm::vec3(tangent.x, tangent.y, tangent.z);

                // bitangent vectors
                auto &bitangent   = mesh->mBitangents[i];
                vertex.bitangents = glm::vec3(bitangent.x, bitangent.y, bitangent.z);
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

        // Move meshobject
        m_meshes.push_back(std::move(meshObject));

        // load material used by mesh
        loadMaterial(aiTextureType_DIFFUSE, mesh->mMaterialIndex);
    }

    void Model::Internal::loadMaterial(aiTextureType type, int materialIndex)
    {
        L_TAG("Model::Internal::loadMaterial");
        L_ASSERT(m_scene->mNumMaterials > materialIndex,
                 "materialIndex exceeds numMaterials in scene");
        auto &material = m_scene->mMaterials[materialIndex];

        // load all textures of type
        for (int i = 0; i < material->GetTextureCount(type); ++i)
        {
            Texture  texture;
            aiString texturePath;
            auto     t = material->GetTexture(type, i, &texturePath);

            /** @todo: load texture here */

            texture.m_name = std::string(texturePath.C_Str());
            L_TRACE("Loaded texture -- {}", texture.m_name);
            m_textures.push_back(std::move(texture));
        }
    }

    Model::Model(const AssetName &name) : m_internal(std::make_unique<Internal>())
    {
        L_TAG("Model::Model");

        L_DEBUG("Loading model: {}", name);

        {
            Assimp::Importer importer;
            const aiScene   *scene = importer.ReadFile(name,
                                                     aiProcess_Triangulate | aiProcess_GenNormals
                                                         | aiProcess_CalcTangentSpace
                                                         | aiProcess_JoinIdenticalVertices);

            if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
            {
                L_THROW_RUNTIME("Error during model import: {}", importer.GetErrorString());
            }

            // Load to internal ptr
            m_internal->m_scene = scene;

            // Use helper function to load model from internal ptr
            m_internal->loadModel();
            // this->loadModel();
            // core::assets::loadNode(scene, scene->mRootNode);

            importer.FreeScene();
        }
    }

    Model::~Model() = default;

    std::vector<Mesh>    &Model::getMeshes() const noexcept { return this->m_internal->m_meshes; }
    std::vector<Texture> &Model::getTextures() const noexcept
    {
        return this->m_internal->m_textures;
    }
} // namespace core::assets
