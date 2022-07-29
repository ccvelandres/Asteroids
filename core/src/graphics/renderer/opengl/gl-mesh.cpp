#include <core/graphics/renderer/opengl/gl-mesh.hpp>

#include <utils/logging.hpp>

static GLuint createVertexBuffer(const assets::Mesh &mesh)
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
    auto &vertice = mesh.getVertices();
    bufferSize    = (vertice.size() * sizeof(Vertex));
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

static GLuint createIndiceBuffer(const assets::Mesh &mesh)
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

struct OpenGLMesh::Internal
{
    const GLuint   vertexBufferId;
    const GLuint   indiceBufferId;
    const uint32_t indiceCount;
    const GLsizei  stride;
    const GLsizei  offsetPosition;
    const GLsizei  offsetNormals;
    const GLsizei  offsetTexCoords;

    Internal(const assets::Mesh &mesh)
        : vertexBufferId(::createVertexBuffer(mesh)),
          indiceBufferId(::createIndiceBuffer(mesh)),
          indiceCount(mesh.getIndices().size()),
          stride(sizeof(Vertex)),
          offsetPosition(offsetof(Vertex, v)),
          offsetNormals(offsetof(Vertex, vn)),
          offsetTexCoords(offsetof(Vertex, uv))
    {
        L_TAG("OpenGLMesh::Internal");
        L_TRACE("Internal resources initialized ({})", static_cast<void *>(this));
    }

    ~Internal()
    {
        L_TAG("OpenGLMesh::~Internal");
        L_TRACE("Internal resources freed ({})", static_cast<void *>(this));
    }
};

OpenGLMesh::OpenGLMesh(const assets::Mesh &mesh) : m_internal(std::make_unique<Internal>(mesh)) {}

OpenGLMesh::OpenGLMesh()                          = default;
OpenGLMesh::OpenGLMesh(OpenGLMesh &&o)            = default;
OpenGLMesh &OpenGLMesh::operator=(OpenGLMesh &&o) = default;
OpenGLMesh::~OpenGLMesh()                         = default;

GLuint   OpenGLMesh::getVertexBufferID() const noexcept { return m_internal->vertexBufferId; }
GLuint   OpenGLMesh::getIndiceBufferID() const noexcept { return m_internal->indiceBufferId; }
uint32_t OpenGLMesh::getIndiceCount() const noexcept { return m_internal->indiceCount; }
GLsizei  OpenGLMesh::getStride() const noexcept { return m_internal->stride; }
GLsizei  OpenGLMesh::getOffsetPosition() const noexcept { return m_internal->offsetPosition; }
GLsizei  OpenGLMesh::getOffsetNormals() const noexcept { return m_internal->offsetNormals; }
GLsizei  OpenGLMesh::getOffsetTexCoords() const noexcept { return m_internal->offsetTexCoords; }