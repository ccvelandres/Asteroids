#include <renderer/opengl/gl-mesh.hpp>

#include <utils/logging.hpp>

GLuint createVertexBuffer( const assets::Mesh &mesh )
{
    std::vector<float> bufferData;

    /** @todo: is it possible to just cast the std::vector<Vertex>? Is it POD? */
    for ( const auto &vertex : mesh.getVertices() )
    {
        /** Vertex coordinates */
        bufferData.push_back( vertex.position.x );
        bufferData.push_back( vertex.position.y );
        bufferData.push_back( vertex.position.z );
        /** Texture Coordinates */
        bufferData.push_back( vertex.texCoordinates.x );
        bufferData.push_back( vertex.texCoordinates.y );
    }

    GLuint bufferId;
    glGenBuffers( 1, &bufferId );
    glBindBuffer( GL_ARRAY_BUFFER, bufferId );
    glBufferData( GL_ARRAY_BUFFER, bufferData.size() * sizeof( float ), bufferData.data(), GL_STATIC_DRAW );

    /** @todo: Error checking */
    return bufferId;
}

GLuint createIndiceBuffer( const assets::Mesh &mesh )
{
    GLuint bufferId;
    glGenBuffers( 1, &bufferId );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, bufferId );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, mesh.getIndices().size() * sizeof( uint32_t ), mesh.getIndices().data(), GL_STATIC_DRAW );

    /** @todo: Error checking */
    return bufferId;
}

struct OpenGLMesh::Internal
{
    const GLuint vertexBufferId;
    const GLuint indiceBufferId;

    Internal( const assets::Mesh &mesh )
        : vertexBufferId( ::createVertexBuffer( mesh ) ),
          indiceBufferId( ::createIndiceBuffer( mesh ) )
    {
        L_TAG( "OpenGLMesh::Internal" );
        L_TRACE( "Internal resources initialized ({})", static_cast<void *>( this ) );
    }

    ~Internal()
    {
        L_TAG( "OpenGLMesh::~Internal" );
        L_TRACE( "Internal resources freed ({})", static_cast<void *>( this ) );
    }
};

OpenGLMesh::OpenGLMesh( const assets::Mesh &mesh ) 
    : m_internal( std::make_unique<Internal>( mesh ) ) {}

OpenGLMesh::OpenGLMesh( OpenGLMesh &&o ) = default;
OpenGLMesh &OpenGLMesh::operator=( OpenGLMesh &&o ) = default;
OpenGLMesh::~OpenGLMesh()                           = default;

GLuint OpenGLMesh::getVertexBufferId() const { return m_internal->vertexBufferId; }
GLuint OpenGLMesh::getIndiceBufferId() const { return m_internal->indiceBufferId; }