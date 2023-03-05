#include <core/graphics/renderer/opengl/gl-pipeline.hpp>
#include <assets/utils.hpp>
#include <utils/logging.hpp>

#include <vector>

struct UniformConfig
{
    GLint  index;
    GLint  size;
    GLenum type;
};

struct AttributeConfig
{
    GLint  index;
    GLint  size;
    GLenum type;
};

static GLuint compileShader(const GLenum shaderType, const std::string &shaderSource)
{
    L_TAG("compileShader");

    GLuint shaderId = glCreateShader(shaderType);

    std::vector<char> shaderSourceData = core::assets::utils::loadBinaryFile(shaderSource);

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

static GLuint compileShader(const GLenum                             shaderType,
                            const core::assets::Shader::ShaderStage &shaderStage)
{
    L_TAG("compileShader");

    const char *shaderData       = shaderStage.binaryData.data();
    GLint       shaderDataLength = shaderStage.binaryData.size();
    GLuint      shaderId         = glCreateShader(shaderType);

    glShaderSource(shaderId, 1, &shaderData, &shaderDataLength);
    glCompileShader(shaderId);

    GLint res;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &res);

    if (res != GL_TRUE)
    {
        GLint messageLength;
        glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &messageLength);
        std::string msg(messageLength + 1, '\0');
        glGetShaderInfoLog(shaderId, messageLength, nullptr, msg.data());
        L_THROW_RUNTIME("{} failed to compile: {}", shaderStage.path, msg);
    }

    return shaderId;
}

static GLuint createPipelineProgram(const core::assets::Shader &shader)
{
    L_TAG("createPipelineProgram");

    GLuint              shaderProgramId = glCreateProgram();
    std::vector<GLuint> shaderIds(shader.stageCount());

    for (auto &shaderStage : shader.getShaderStages())
    {
        GLenum shaderType = OpenGLPipeline::shaderTypeFromType(shaderStage.shaderType);
        GLuint shaderId   = ::compileShader(shaderType, shaderStage);
        shaderIds.push_back(shaderId);
        glAttachShader(shaderProgramId, shaderId);
        L_TRACE("Compiled shader. {}: {}", shaderId, shaderStage.path);
    }

    /** Bind vertex indices to expected indices */
    glBindAttribLocation(shaderProgramId, 0, "a_v");  // geometry vertice
    glBindAttribLocation(shaderProgramId, 1, "a_vn"); // normals
    glBindAttribLocation(shaderProgramId, 2, "a_vt"); // texture coordinates
    glBindAttribLocation(shaderProgramId, 3, "a_t");  // tangents
    glBindAttribLocation(shaderProgramId, 5, "a_bt"); // bitangents

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

    /** Bind vertex indices to expected indices */
    glBindAttribLocation(shaderProgramId, 0, "a_v");  // geometry vertice
    glBindAttribLocation(shaderProgramId, 1, "a_vn"); // normals
    glBindAttribLocation(shaderProgramId, 2, "a_vt"); // texture coordinates
    glBindAttribLocation(shaderProgramId, 3, "a_t");  // tangents
    glBindAttribLocation(shaderProgramId, 5, "a_bt"); // bitangents

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

std::unordered_map<std::string, UniformConfig> getProgramUniforms(GLuint shaderProgramID)
{
    L_TAG("OpenGLPipeline::getProgramUniforms(programID)");
    std::unordered_map<std::string, UniformConfig> uniformMap;

    constexpr GLsizei uniformNameSize = 32;
    GLchar            name[uniformNameSize];
    GLsizei           nameLength;
    GLint             size;
    GLenum            type;

    GLint uniformCount;
    glGetProgramiv(shaderProgramID, GL_ACTIVE_UNIFORMS, &uniformCount);
    L_TRACE("Retrieving uniforms for pipeline: {}", shaderProgramID);
    for (GLint i = 0; i < uniformCount; i++)
    {
        glGetActiveUniform(shaderProgramID, i, uniformNameSize, &nameLength, &size, &type, name);
        L_TRACE("Uniform: {}: {}; Type: 0x{:X}; Size: {}", i, name, type, size);

        uniformMap[std::string(name)] = UniformConfig{.index = i, .size = size, .type = type};
    }
    return std::move(uniformMap);
}

std::unordered_map<std::string, AttributeConfig> getProgramAttributes(GLuint shaderProgramID)
{
    L_TAG("OpenGLPipeline::getProgramAttributes(programID)");
    std::unordered_map<std::string, AttributeConfig> attributeMap;

    constexpr GLsizei attributeNameSize = 32;
    GLchar            name[attributeNameSize];
    GLsizei           nameLength;
    GLint             size;
    GLenum            type;

    GLint attributeCount;
    glGetProgramiv(shaderProgramID, GL_ACTIVE_ATTRIBUTES, &attributeCount);
    L_TRACE("Retrieving attributes for pipeline: {}", shaderProgramID);
    for (GLint i = 0; i < attributeCount; i++)
    {
        glGetActiveAttrib(shaderProgramID, i, attributeNameSize, &nameLength, &size, &type, name);
        L_TRACE("Attribute: {}: {}; Type: 0x{:X}; Size: {}", i, name, type, size);

        attributeMap[std::string(name)] = AttributeConfig{.index = i, .size = size, .type = type};
    }
    return std::move(attributeMap);
}

struct OpenGLPipeline::Internal
{
    const std::string                                      pipelineName;
    const GLuint                                           shaderProgramId;
    const GLint                                            su_mvp;
    const GLint                                            sa_vertices;
    const GLint                                            sa_vertexNormals;
    const GLint                                            sa_textureVertice;
    const GLint                                            sa_tangents;
    const GLint                                            sa_bitangents;
    const std::unordered_map<std::string, UniformConfig>   uniforms;
    const std::unordered_map<std::string, AttributeConfig> attributes;

    Internal(const std::string &name, const std::vector<ShaderStage> &shaderStages)
        : pipelineName(name),
          shaderProgramId(::createPipelineProgram(shaderStages)),
          su_mvp(glGetUniformLocation(shaderProgramId, "u_mvp")),
          sa_vertices(glGetAttribLocation(shaderProgramId, "a_v")),
          sa_vertexNormals(glGetAttribLocation(shaderProgramId, "a_vn")),
          sa_textureVertice(glGetAttribLocation(shaderProgramId, "a_vt")),
          sa_tangents(glGetAttribLocation(shaderProgramId, "a_t")),
          sa_bitangents(glGetAttribLocation(shaderProgramId, "a_bt")),
          uniforms(::getProgramUniforms(shaderProgramId)),
          attributes(::getProgramAttributes(shaderProgramId))
    {
        L_TAG("OpenGLPipeline::Internal");
        L_DEBUG("Pipeline \"{}\" created with {} shaders", pipelineName, shaderStages.size());
        L_TRACE("Internal resources initialized ({})", static_cast<void *>(this));
    }

    Internal(const core::assets::Shader &shader)
        : pipelineName(shader.name()),
          shaderProgramId(::createPipelineProgram(shader)),
          su_mvp(glGetUniformLocation(shaderProgramId, "u_mvp")),
          sa_vertices(glGetAttribLocation(shaderProgramId, "a_v")),
          sa_vertexNormals(glGetAttribLocation(shaderProgramId, "a_vn")),
          sa_textureVertice(glGetAttribLocation(shaderProgramId, "a_vt")),
          sa_tangents(glGetAttribLocation(shaderProgramId, "a_t")),
          sa_bitangents(glGetAttribLocation(shaderProgramId, "a_bt")),
          uniforms(::getProgramUniforms(shaderProgramId)),
          attributes(::getProgramAttributes(shaderProgramId))
    {
        L_TAG("OpenGLPipeline::Internal");
        L_DEBUG("Pipeline \"{}\" created with {} shaders", pipelineName, shader.stageCount());
        L_TRACE("Internal resources initialized ({})", static_cast<void *>(this));
    }

    void setUniform(const UniformConfig &uniform,
                    void                *data,
                    std::size_t          dataLength,
                    GLboolean            transpose = GL_FALSE)
    {
        L_TAG("OpenGLPipeline::setUniform");

        /** Sanity check for the data lengths
         *  lambda so compiler can do what's best and cleaner code
         */
        auto sizeCheck = [this](std::size_t a, std::size_t b) {
            L_ASSERT(a <= b,
                     "Pipeline {}: Expected data length ({}), got ({})",
                     this->shaderProgramId,
                     a,
                     b);
        };

        switch (uniform.type)
        {
        case GL_FLOAT_VEC2:
            sizeCheck(uniform.size * sizeof(GLfloat) * 2, dataLength);
            glUniform2fv(uniform.index, uniform.size, static_cast<GLfloat *>(data));
            break;
        case GL_FLOAT_VEC3:
            sizeCheck(uniform.size * sizeof(GLfloat) * 3, dataLength);
            glUniform3fv(uniform.index, uniform.size, static_cast<GLfloat *>(data));
            break;
        case GL_FLOAT_VEC4:
            sizeCheck(uniform.size * sizeof(GLfloat) * 4, dataLength);
            glUniform4fv(uniform.index, uniform.size, static_cast<GLfloat *>(data));
            break;
        case GL_BOOL:
            sizeCheck(uniform.size * sizeof(GLint) * 1, dataLength);
            glUniform1iv(uniform.index, uniform.size, static_cast<GLint *>(data));
            break;
        case GL_INT_VEC2:
        case GL_BOOL_VEC2:
            sizeCheck(uniform.size * sizeof(GLint) * 2, dataLength);
            glUniform2iv(uniform.index, uniform.size, static_cast<GLint *>(data));
            break;
        case GL_INT_VEC3:
        case GL_BOOL_VEC3:
            sizeCheck(uniform.size * sizeof(GLint) * 3, dataLength);
            glUniform3iv(uniform.index, uniform.size, static_cast<GLint *>(data));
            break;
        case GL_INT_VEC4:
        case GL_BOOL_VEC4:
            sizeCheck(uniform.size * sizeof(GLint) * 4, dataLength);
            glUniform4iv(uniform.index, uniform.size, static_cast<GLint *>(data));
            break;
        case GL_FLOAT_MAT2:
            sizeCheck(uniform.size * sizeof(GLfloat) * 4, dataLength);
            glUniformMatrix2fv(uniform.index, uniform.size, transpose, static_cast<GLfloat *>(data));
            break;
        case GL_FLOAT_MAT3:
            sizeCheck(uniform.size * sizeof(GLfloat) * 9, dataLength);
            glUniformMatrix3fv(uniform.index, uniform.size, transpose, static_cast<GLfloat *>(data));
            break;
        case GL_FLOAT_MAT4:
            sizeCheck(uniform.size * sizeof(GLfloat) * 16, dataLength);
            glUniformMatrix4fv(uniform.index, uniform.size, transpose, static_cast<GLfloat *>(data));
            break;
        default:
            L_THROW_LOGIC("Unhandled GLSL Uniform type: 0x{:X}", uniform.type);
        }
    }

    ~Internal()
    {
        L_TAG("OpenGLPipeline::~Internal");
        L_TRACE("Internal resources freed ({})", static_cast<void *>(this));
    }

    void render(const OpenGLMesh &mesh, const glm::mat4 &mvp)
    {
        L_TAG("OpenGLPipeline::render(OpenGLMesh)");

        GLsizei stride = mesh.getStride();

        /** Render in wireframe */
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        /** Use pipeline program */
        glUseProgram(shaderProgramId);

        /** Pass mvp to shader program */
        glUniformMatrix4fv(su_mvp, 1, GL_FALSE, &mvp[0][0]);

        /** Bind buffers and default texture */
        glBindBuffer(GL_ARRAY_BUFFER, mesh.getVertexBufferID());
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.getIndiceBufferID());
        // glBindTexture(GL_TEXTURE_2D, 0);

        /** Configure and pass geometry vertices */
        GLsizei offsetV = mesh.getOffsetPosition();
        glEnableVertexAttribArray(sa_vertices);
        glVertexAttribPointer(sa_vertices,
                              3,
                              GL_FLOAT,
                              GL_FALSE,
                              stride,
                              reinterpret_cast<const void *>(offsetV));

        /** Configure and pass vertex normals */
        GLsizei offsetVN = mesh.getOffsetNormals();
        glEnableVertexAttribArray(sa_vertexNormals);
        glVertexAttribPointer(sa_vertexNormals,
                              3,
                              GL_FLOAT,
                              GL_FALSE,
                              stride,
                              reinterpret_cast<const void *>(offsetVN));

        /** Configure and pass uv coordinates */
        GLsizei offsetVT = mesh.getOffsetTexCoords();
        glEnableVertexAttribArray(sa_textureVertice);
        glVertexAttribPointer(sa_textureVertice,
                              3,
                              GL_FLOAT,
                              GL_FALSE,
                              stride,
                              reinterpret_cast<const void *>(offsetVT));

        /** Render mesh with wireframe */
        glDrawElements(GL_TRIANGLES,
                       mesh.getIndiceCount(),
                       GL_UNSIGNED_INT,
                       reinterpret_cast<const GLvoid *>(0));

        glDisableVertexAttribArray(sa_vertices);
        glDisableVertexAttribArray(sa_vertexNormals);
        glDisableVertexAttribArray(sa_textureVertice);
    }

    void render(const OpenGLMesh &mesh, const OpenGLTexture &texture, const glm::mat4 &mvp)
    {
        L_TAG("OpenGLPipeline::render(OpenGLMesh, OpenGLTexture)");

        GLsizei stride    = mesh.getStride();
        GLsizei offsetV   = mesh.getOffsetPosition();
        GLsizei offsetVN  = mesh.getOffsetNormals();
        GLsizei offsetVT  = mesh.getOffsetTexCoords();
        GLuint  textureID = texture.getTextureID();

        /** Render in wireframe */
        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        /** Use pipeline program */
        glUseProgram(shaderProgramId);

        /** Pass mvp to shader program */
        glUniformMatrix4fv(su_mvp, 1, GL_FALSE, &mvp[0][0]);

        /** Bind buffers and default texture */
        glBindBuffer(GL_ARRAY_BUFFER, mesh.getVertexBufferID());
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.getIndiceBufferID());
        glBindTexture(GL_TEXTURE_2D, textureID);

        /** Configure and pass geometry vertices */
        glEnableVertexAttribArray(sa_vertices);
        glVertexAttribPointer(sa_vertices,
                              3,
                              GL_FLOAT,
                              GL_FALSE,
                              stride,
                              reinterpret_cast<const void *>(offsetV));

        /** Configure and pass vertex normals */
        glEnableVertexAttribArray(sa_vertexNormals);
        glVertexAttribPointer(sa_vertexNormals,
                              3,
                              GL_FLOAT,
                              GL_FALSE,
                              stride,
                              reinterpret_cast<const void *>(offsetVN));

        /** Configure and pass uv coordinates */
        glEnableVertexAttribArray(sa_textureVertice);
        glVertexAttribPointer(sa_textureVertice,
                              2,
                              GL_FLOAT,
                              GL_FALSE,
                              stride,
                              reinterpret_cast<const void *>(offsetVT));

        /** Render mesh with wireframe */
        glDrawElements(GL_TRIANGLES,
                       mesh.getIndiceCount(),
                       GL_UNSIGNED_INT,
                       reinterpret_cast<const GLvoid *>(0));

        glDisableVertexAttribArray(sa_vertices);
        glDisableVertexAttribArray(sa_vertexNormals);
        glDisableVertexAttribArray(sa_textureVertice);
    }

    void render(RenderInfo &renderInfo)
    {
        L_TAG("OpenGLPipeline::render(&renderInfo)");

        glUseProgram(shaderProgramId);

        // Bind textures to each texture unit
        // glBindTextures(0, renderInfo.textures.size(), renderInfo.textures.data());
        glBindTexture(GL_TEXTURE_2D, renderInfo.textures[0]);
        // Bind buffers
        for (auto &buffer : renderInfo.buffers)
            glBindBuffer(buffer.first, buffer.second);

        for (auto &uniform : renderInfo.uniforms)
        {
            /** check first if pipeline has uniform with matching name
             * @todo: maybe change string names with compile time hashes
             */
            auto uniformConfig = uniforms.find(uniform.first);
            if (uniformConfig == uniforms.end()) continue;
            // L_THROW_LOGIC("Pipeline {} does not have uniform named {}", shaderProgramId, uniform.first);

            /** Sanity check that the expected type matches */
            L_ASSERT(uniformConfig->second.type == uniform.second.type,
                     "ShaderUniformType does not match given uniform type");
            /** Use helper function for setting uniform */
            setUniform(uniformConfig->second,
                       uniform.second.data,
                       uniform.second.dataSize,
                       uniform.second.matrixTransponse);
        }

        glDrawElements(renderInfo.drawInfo.drawMode,
                       renderInfo.drawInfo.indiceCount,
                       renderInfo.drawInfo.indiceType,
                       static_cast<GLvoid *>(renderInfo.drawInfo.indiceLocation));
    }
};

OpenGLPipeline::OpenGLPipeline(const std::string &name, const std::vector<ShaderStage> &shaderStages)
    : m_internal(std::make_unique<Internal>(name, shaderStages))
{
}

OpenGLPipeline::OpenGLPipeline(const core::assets::Shader &shader)
    : m_internal(std::make_unique<Internal>(shader))
{
    L_TAG("OpenGLPipeline::OpenGLPipeline");
}

OpenGLPipeline::OpenGLPipeline()                              = default;
OpenGLPipeline::OpenGLPipeline(OpenGLPipeline &&o)            = default;
OpenGLPipeline &OpenGLPipeline::operator=(OpenGLPipeline &&o) = default;
OpenGLPipeline::~OpenGLPipeline()                             = default;

/** @todo: need to deprecate these functions */
void OpenGLPipeline::render(const OpenGLMesh &mesh, const glm::mat4 &mvp) const
{
    m_internal->render(mesh, mvp);
}
void OpenGLPipeline::render(const OpenGLMesh    &mesh,
                            const OpenGLTexture &texture,
                            const glm::mat4     &mvp) const
{
    m_internal->render(mesh, texture, mvp);
}

void OpenGLPipeline::render(RenderInfo &renderInfo) const { m_internal->render(renderInfo); }

GLenum OpenGLPipeline::shaderTypeFromName(const std::string &name)
{
    L_TAG("OpenGLPipeline::shaderTypeFromName");

    const std::unordered_map<std::string, GLenum> shaderExtensions = {
        {".vert",  GL_VERTEX_SHADER         }, // Vertex Shader
        {".vs",    GL_VERTEX_SHADER         }, // Vertex Shader
        {".frag",  GL_FRAGMENT_SHADER       }, // Fragment Shader
        {".fs",    GL_FRAGMENT_SHADER       }, // Fragment Shader
        {".gs",    GL_GEOMETRY_SHADER       }, // Geometry Shader
        {".geom",  GL_GEOMETRY_SHADER       }, // Geometry Shader
        {".comp",  GL_COMPUTE_SHADER        }, // Compute Shader
        {".tesc",  GL_TESS_CONTROL_SHADER   }, // Tesselation Control Shader
        {".tese",  GL_TESS_EVALUATION_SHADER}, // Tesselation Evaluation Shader
        {".rgen",  GL_FALSE                 }, // Ray Generation shader
        {".rint",  GL_FALSE                 }, // Ray Intersection shader
        {".rahit", GL_FALSE                 }, // Ray Any-hit shader
        {".rchit", GL_FALSE                 }, // Ray Closest-hit shader
        {".rmiss", GL_FALSE                 }, // Ray Miss shader
        {".rcall", GL_FALSE                 }, // Ray Callable shader
        {".mesh",  GL_FALSE                 }, // Mesh Shader
        {".task",  GL_FALSE                 }  // Task Shader
    };

    auto it = shaderExtensions.find(name);
    if (it != shaderExtensions.end())
        return (*it).second;
    else
        L_THROW_RUNTIME("Could not identify shader type: {}", name);
}

GLenum OpenGLPipeline::shaderTypeFromType(const core::assets::Shader::ShaderType &shaderType)
{
    L_TAG("OpenGLPipeline::shaderTypeFromName");

    typedef typename core::assets::Shader::ShaderType ShaderType;

    const std::unordered_map<core::assets::Shader::ShaderType, GLenum> shaderExtensions = {
        {ShaderType::Vertex,          GL_VERTEX_SHADER         }, // Vertex Shader
        {ShaderType::Vertex,          GL_VERTEX_SHADER         }, // Vertex Shader
        {ShaderType::Fragment,        GL_FRAGMENT_SHADER       }, // Fragment Shader
        {ShaderType::Fragment,        GL_FRAGMENT_SHADER       }, // Fragment Shader
        {ShaderType::Geometry,        GL_GEOMETRY_SHADER       }, // Geometry Shader
        {ShaderType::Geometry,        GL_GEOMETRY_SHADER       }, // Geometry Shader
        {ShaderType::Compute,         GL_COMPUTE_SHADER        }, // Compute Shader
        {ShaderType::TesselationCtrl, GL_TESS_CONTROL_SHADER   }, // Tesselation Control  Shader
        {ShaderType::TesselationEval, GL_TESS_EVALUATION_SHADER}, // Tesselation Evaluation Shader
        {ShaderType::RayGeneration,   GL_FALSE                 }, // Ray Generation shader
        {ShaderType::RayIntersection, GL_FALSE                 }, // Ray Intersection shader
        {ShaderType::RayAnyHit,       GL_FALSE                 }, // Ray Any-hit shader
        {ShaderType::RayClosestHit,   GL_FALSE                 }, // Ray Closest-hit shader
        {ShaderType::RayMiss,         GL_FALSE                 }, // Ray Miss shader
        {ShaderType::RayCallable,     GL_FALSE                 }, // Ray Callable shader
        {ShaderType::Mesh,            GL_FALSE                 }, // Mesh Shader
        {ShaderType::Task,            GL_FALSE                 }  // Task Shader
    };

    auto it = shaderExtensions.find(shaderType);
    if (it != shaderExtensions.end())
        return (*it).second;
    else
        L_THROW_RUNTIME("Could not identify shader type: {}", static_cast<int>(shaderType));
}