#include <renderer/opengl/gl-pipeline.hpp>
#include <assets/utils.hpp>
#include <utils/logging.hpp>

#include <vector>

static GLuint compileShader(const GLenum shaderType, const std::string &shaderSource)
{
    L_TAG("compileShader");

    GLuint shaderId = glCreateShader(shaderType);

    std::vector<char> shaderSourceData = assets::utils::loadBinaryFile(shaderSource);

    const char *shaderData         = shaderSourceData.data();
    GLint       shaderSourceLength = shaderSourceData.size();
    glShaderSource(shaderId, 1, &shaderData, &shaderSourceLength);
    glCompileShader(shaderId);

    GLint res;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &res);

    if (res != GL_TRUE)
    {
        GLint messageLength;
        glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &messageLength);
        std::string msg(messageLength + 1, '\0');
        glGetShaderInfoLog(shaderId, messageLength, nullptr, msg.data());
        L_THROW_RUNTIME("{} failed to compile: {}", shaderSource, msg);
    }

    return shaderId;
}

static GLuint createPipelineProgram(const std::vector<OpenGLPipeline::ShaderStage> &shaderStages)
{
    L_TAG("createPipelineProgram");

    /** Create shader program */
    GLuint              shaderProgramId = glCreateProgram();
    std::vector<GLuint> shaderIds(shaderStages.size());

    /** Compile and attach each shader stages to program */
    for (const auto &stage : shaderStages)
    {
        GLuint shaderId = ::compileShader(stage.shaderType, stage.shaderSource);
        shaderIds.push_back(shaderId);
        glAttachShader(shaderProgramId, shaderId);
    }

    /** Link and check result */
    GLint res;
    glLinkProgram(shaderProgramId);
    glGetProgramiv(shaderProgramId, GL_LINK_STATUS, &res);

    if (res != GL_TRUE)
    {
        GLint messageLength;
        glGetProgramiv(shaderProgramId, GL_INFO_LOG_LENGTH, &messageLength);
        std::string msg(messageLength + 1, '\0');
        glGetProgramInfoLog(shaderProgramId, messageLength, nullptr, msg.data());
        L_THROW_RUNTIME("Shader program failed to link: {}", msg);
    }

    /** Cleanup files */
    for (const auto &shader : shaderIds)
    {
        glDetachShader(shaderProgramId, shader);
        glDeleteShader(shader);
    }

    return shaderProgramId;
}

struct OpenGLPipeline::Internal
{
    const std::string pipelineName;
    const GLuint      shaderProgramId;
    const GLint       su_mvp;
    const GLint       sa_vertices;
    const GLint       sa_vertexNormals;
    const GLint       sa_textureVertice;

    Internal(const std::string &name, const std::vector<ShaderStage> &shaderStages)
        : pipelineName(name),
          shaderProgramId(::createPipelineProgram(shaderStages)),
          su_mvp(glGetUniformLocation(shaderProgramId, "u_mvp")),
          sa_vertices(glGetAttribLocation(shaderProgramId, "a_v")),
          sa_vertexNormals(glGetAttribLocation(shaderProgramId, "a_vn")),
          sa_textureVertice(glGetAttribLocation(shaderProgramId, "a_vt"))
    {
        L_TAG("OpenGLPipeline::Internal");
        L_DEBUG("Pipeline \"{}\" created with {} shaders", pipelineName, shaderStages.size());
        L_TRACE("Internal resources initialized ({})", static_cast<void *>(this));
    }

    ~Internal()
    {
        L_TAG("OpenGLPipeline::~Internal");
        L_TRACE("Internal resources freed ({})", static_cast<void *>(this));
    }

    void render(const OpenGLMesh &mesh, const glm::mat4 &mvp)
    {
        L_TAG("OpenGLPipeline::render(OpenGLMesh)");

        GLsizei stride   = mesh.getStride();
        GLsizei offsetV  = mesh.getOffsetPosition();
        GLsizei offsetVN = mesh.getOffsetNormals();
        GLsizei offsetVT = mesh.getOffsetTexCoords();

        /** Render in wireframe */
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        /** Use pipeline program */
        glUseProgram(shaderProgramId);

        /** Pass mvp to shader program */
        glUniformMatrix4fv(su_mvp, 1, GL_FALSE, &mvp[0][0]);
        glBindBuffer(GL_ARRAY_BUFFER, mesh.getVertexBufferID());
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.getIndiceBufferID());

        /** Configure and pass geometry vertices */
        glEnableVertexAttribArray(sa_vertices);
        glVertexAttribPointer(sa_vertices, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<const void *>(offsetV));

        /** Configure and pass vertex normals */
        glEnableVertexAttribArray(sa_vertexNormals);
        glVertexAttribPointer(sa_vertexNormals, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<const void *>(offsetVN));

        /** Configure and pass uv coordinates */
        glEnableVertexAttribArray(sa_textureVertice);
        glVertexAttribPointer(sa_textureVertice, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<const void *>(offsetVT));

        /** Render mesh with wireframe */
        glDrawElements(GL_TRIANGLES, mesh.getIndiceCount(), GL_UNSIGNED_INT, reinterpret_cast<const GLvoid *>(0));

        glDisableVertexAttribArray(sa_vertices);
        glDisableVertexAttribArray(sa_vertexNormals);
        glDisableVertexAttribArray(sa_textureVertice);
    }

    void render(const OpenGLMesh &mesh, const OpenGLTexture &texture, const glm::mat4 &mvp) {
        L_TAG("OpenGLPipeline::render(OpenGLMesh, OpenGLTexture)");

    }
};

OpenGLPipeline::OpenGLPipeline(const std::string &name, const std::vector<ShaderStage> &shaderStages)
    : m_internal(std::make_unique<Internal>(name, shaderStages))
{
}

OpenGLPipeline::OpenGLPipeline()                              = default;
OpenGLPipeline::OpenGLPipeline(OpenGLPipeline &&o)            = default;
OpenGLPipeline &OpenGLPipeline::operator=(OpenGLPipeline &&o) = default;
OpenGLPipeline::~OpenGLPipeline()                             = default;

void OpenGLPipeline::render(const OpenGLMesh &mesh, const glm::mat4 &mvp) const { m_internal->render(mesh, mvp); }
void OpenGLPipeline::render(const OpenGLMesh &mesh, const OpenGLTexture &texture, const glm::mat4 &mvp) const
{
    m_internal->render(mesh, texture, mvp);
}

GLenum OpenGLPipeline::shaderTypeFromName(const std::string &name)
{
    L_TAG("OpenGLPipeline::shaderTypeFromName");

    const std::unordered_map<std::string, GLenum> shaderExtensions = {
        { ".vert",          GL_VERTEX_SHADER}, // Vertex Shader
        {   ".vs",          GL_VERTEX_SHADER}, // Vertex Shader
        { ".frag",        GL_FRAGMENT_SHADER}, // Fragment Shader
        {   ".fs",        GL_FRAGMENT_SHADER}, // Fragment Shader
        {   ".gs",        GL_GEOMETRY_SHADER}, // Geometry Shader
        { ".geom",        GL_GEOMETRY_SHADER}, // Geometry Shader
        { ".comp",         GL_COMPUTE_SHADER}, // Compute Shader
        { ".tesc",    GL_TESS_CONTROL_SHADER}, // Tesselation Control Shader
        { ".tese", GL_TESS_EVALUATION_SHADER}, // Tesselation Evaluation Shader
        { ".rgen",                  GL_FALSE}, // Ray Generation shader
        { ".rint",                  GL_FALSE}, // Ray Intersection shader
        {".rahit",                  GL_FALSE}, // Ray Any-hit shader
        {".rchit",                  GL_FALSE}, // Ray Closest-hit shader
        {".rmiss",                  GL_FALSE}, // Ray Miss shader
        {".rcall",                  GL_FALSE}, // Ray Callable shader
        { ".mesh",                  GL_FALSE}, // Mesh Shader
        { ".task",                  GL_FALSE}  // Task Shader
    };

    auto it = shaderExtensions.find(name);
    if (it != shaderExtensions.end())
        return (*it).second;
    else
        L_THROW_RUNTIME("Could not identify shader type: {}", name);
}