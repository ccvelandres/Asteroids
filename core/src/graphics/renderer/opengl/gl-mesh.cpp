#include <core/graphics/renderer/opengl/gl-mesh.hpp>

#include <utils/logging.hpp>

static GLuint createVertexBuffer(const core::assets::Mesh &mesh)
{
    L_TAG("OpenGLMesh::createVertexBuffer");
    GLsizeiptr  bufferSize    = 0;
    const void *bufferDataPtr = nullptr;

    // std::vector<float> bufferData;
    // for (const auto &vertex : mesh.getVertices())
    // {
    //     /** Vertex coordinates */
    //     bufferData.push_back(vertex.v.x);
    //     bufferData.push_back(vertex.v.y);
    //     bufferData.push_back(vertex.v.z);
    //     /** Vertex normals */
    //     bufferData.push_back(vertex.vn.x);
    //     bufferData.push_back(vertex.vn.y);
    //     bufferData.push_back(vertex.vn.z);
    //     /** Texture Coordinates */
    //     bufferData.push_back(vertex.uv.x);
    //     bufferData.push_back(vertex.uv.y);
    // }
    // bufferSize = bufferData.size() * sizeof(float);
    // bufferDataPtr = bufferData.data();

    /** @todo: is it possible to just cast the std::vector<Vertex>? Is it POD? */
    const std::vector<OpenGLMesh::VertexType> &vertice = mesh.getVertices();
    bufferSize    = (vertice.size() * sizeof(OpenGLMesh::VertexType));
    bufferDataPtr = reinterpret_cast<const void *>(vertice.data());

    L_DEBUG("Loading {} bytes", bufferSize);

    GLuint bufferId;
    {
        PROFILER_BLOCK("OpenGLMesh::createVertexBuffer::glBufferData");
        glGenBuffers(1, &bufferId);
        glBindBuffer(GL_ARRAY_BUFFER, bufferId);
        glBufferData(GL_ARRAY_BUFFER, bufferSize, bufferDataPtr, GL_STATIC_DRAW);
    }

    /** @todo: Error checking */
    return bufferId;
}

static GLuint createIndiceBuffer(const core::assets::Mesh &mesh)
{
    L_TAG("OpenGLMesh::createIndiceBuffer");

    GLuint bufferId;
    glGenBuffers(1, &bufferId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 mesh.getIndices().size() * sizeof(uint32_t),
                 mesh.getIndices().data(),
                 GL_STATIC_DRAW);

    /** @todo: Error checking */
    return bufferId;
}

static OpenGLMesh::VertexInfo createGLMesh(const core::assets::Mesh &mesh)
{
    L_TAG("OpenGLMesh::createGLMesh(&mesh)");

    OpenGLMesh::VertexInfo vertexInfo;
    vertexInfo.vertexBufferID   = ::createVertexBuffer(mesh);
    vertexInfo.indiceBufferID   = ::createIndiceBuffer(mesh);
    vertexInfo.indiceCount      = static_cast<uint32_t>(mesh.getIndices().size());
    vertexInfo.stride           = sizeof(OpenGLMesh::VertexType);
    vertexInfo.offsetPosition   = offsetof(OpenGLMesh::VertexType, v);
    vertexInfo.offsetNormals    = offsetof(OpenGLMesh::VertexType, vn);
    vertexInfo.offsetTexCoords  = offsetof(OpenGLMesh::VertexType, uv);
    vertexInfo.offsetTangents   = offsetof(OpenGLMesh::VertexType, tangents);
    vertexInfo.offsetBitangents = offsetof(OpenGLMesh::VertexType, bitangents);
    vertexInfo.hasTangents      = mesh.hasTangents();
    vertexInfo.hasBitangents    = mesh.hasBitangents();

    /** @todo: remove tangents, bitangents from stride if mesh does not have them */
    /** @todo support for vertex buffer with no normals */

    /** Configure and pass geometry vertices */
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexInfo.stride, reinterpret_cast<const void *>(vertexInfo.offsetPosition));

    /** Configure and pass vertex normals */
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertexInfo.stride, reinterpret_cast<const void *>(vertexInfo.offsetNormals));

    /** Configure and pass uv coordinates */
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, vertexInfo.stride, reinterpret_cast<const void *>(vertexInfo.offsetTexCoords));

    if(vertexInfo.hasTangents)
    {
        /** Configure and pass tangents */
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, vertexInfo.stride, reinterpret_cast<const void *>(vertexInfo.offsetTangents));
    }

    if(vertexInfo.hasBitangents)
    {
        /** Configure and pass bitangents */
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, vertexInfo.stride, reinterpret_cast<const void *>(vertexInfo.offsetBitangents));
    }

    return vertexInfo;
}

OpenGLMesh::OpenGLMesh(const core::assets::Mesh &mesh) : m_vertexInfo(::createGLMesh(mesh))
{
    L_TAG("OpenGLMesh::Internal");
    L_TRACE("Internal resources initialized ({})", static_cast<void *>(this));
}

OpenGLMesh::OpenGLMesh()                          = default;
OpenGLMesh::OpenGLMesh(OpenGLMesh &&o)            = default;
OpenGLMesh &OpenGLMesh::operator=(OpenGLMesh &&o) = default;
OpenGLMesh::~OpenGLMesh()                         = default;

const GLuint                 &OpenGLMesh::getVertexBufferID() const noexcept { return m_vertexInfo.vertexBufferID; }
const GLuint                 &OpenGLMesh::getIndiceBufferID() const noexcept { return m_vertexInfo.indiceBufferID; }
const uint32_t               &OpenGLMesh::getIndiceCount() const noexcept { return m_vertexInfo.indiceCount; }
const GLsizei                &OpenGLMesh::getStride() const noexcept { return m_vertexInfo.stride; }
const GLsizei                &OpenGLMesh::getOffsetPosition() const noexcept { return m_vertexInfo.offsetPosition; }
const GLsizei                &OpenGLMesh::getOffsetNormals() const noexcept { return m_vertexInfo.offsetNormals; }
const GLsizei                &OpenGLMesh::getOffsetTexCoords() const noexcept { return m_vertexInfo.offsetTexCoords; }
const OpenGLMesh::VertexInfo &OpenGLMesh::getVertexInfo() const noexcept { return m_vertexInfo; }