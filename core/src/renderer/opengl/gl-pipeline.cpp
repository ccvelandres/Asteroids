#include <renderer/opengl/gl-pipeline.hpp>
#include <assets/assets.hpp>
#include <utils/logging.hpp>

#include <vector>

GLuint compileShader(const GLenum shaderType, const std::string &shaderSource)
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
        L_THROW_RUNTIME("Shader failed to compile: {}", msg);
    }

    return shaderId;
}

GLuint createPipelineProgram(const std::vector<OpenGLPipeline::ShaderStage> &shaderStages)
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
    const GLuint  shaderProgramId;
    const GLuint  s_uniformLocationMVP;
    const GLuint  s_vertexPosition;
    const GLuint  s_textureCoord;
    const GLsizei stride;
    const GLsizei offsetPosition;
    const GLsizei offsetTexCoord;

    Internal(const std::vector<ShaderStage> &shaderStages)
        : shaderProgramId(::createPipelineProgram(shaderStages)),
          s_uniformLocationMVP(glGetUniformLocation(shaderProgramId, "u_mvp")),
          s_vertexPosition(glGetAttribLocation(shaderProgramId, "a_vertexPosition")),
          s_textureCoord(glGetAttribLocation(shaderProgramId, "a_texCoord")),
          stride(5 * sizeof(float)),
          offsetPosition(0),
          offsetTexCoord(3 * sizeof(float))
    {
        L_TAG("OpenGLPipeline::Internal");
        L_TRACE("Internal resources initialized ({})", static_cast<void *>(this));
    }

    ~Internal()
    {
        L_TAG("OpenGLPipeline::~Internal");
        L_TRACE("Internal resources freed ({})", static_cast<void *>(this));
    }

    void render(const OpenGLMesh &mesh, const glm::mat4 &mvp)
    {
        /** Use pipeline program */
        glUseProgram(shaderProgramId);

        /** Pass mvp to shader program */
        glUniformMatrix4fv(s_uniformLocationMVP, 1, GL_FALSE, &mvp[0][0]);

        /** Configure and pass s_vertexPosition */
        glEnableVertexAttribArray(s_vertexPosition);
        glVertexAttribPointer(s_vertexPosition, 3, GL_FLOAT, GL_FALSE, stride, (GLvoid *)(&offsetPosition));

        /** Configure and pass the s_textureCoord */
        glEnableVertexAttribArray(s_textureCoord);
        glVertexAttribPointer(s_textureCoord, 2, GL_FLOAT, GL_FALSE, stride, (GLvoid *)(&s_textureCoord));

        /** Apply texture to mesh */
        // texture.bind()

        // glBindBuffer(GL_ARRAY_BUFFER,)
    }
};

OpenGLPipeline::OpenGLPipeline(const std::vector<ShaderStage> &shaderStages)
    : m_internal(std::make_unique<Internal>(shaderStages))
{
}

OpenGLPipeline::OpenGLPipeline(OpenGLPipeline &&o)            = default;
OpenGLPipeline &OpenGLPipeline::operator=(OpenGLPipeline &&o) = default;
OpenGLPipeline::~OpenGLPipeline()                             = default;

void OpenGLPipeline::render(const OpenGLMesh &mesh, const glm::mat4 &mvp) const { m_internal->render(mesh, mvp); }