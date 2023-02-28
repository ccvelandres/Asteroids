#pragma once

/**
 * @file core/renderer/opengl/gl-pipeline.hpp
 * @author Cedric Velandres (ccvelandres@gmail.com)
 *
 * @addtogroup OpenGL
 * @{
 */

#include <memory>
#include <vector>
#include <string>

#include <glm/glm.hpp>

#include "../../shader.hpp"
#include "gl-wrapper.hpp"
#include "gl-mesh.hpp"
#include "gl-texture.hpp"

/**
 * @brief OpenGL Rendering pipeline
 *
 * Vertex Attributes are explicitly bound with:
 *  Index 0: Geometry vertice
 *  Index 1: Vertex Normals
 *  Index 2: Texture Coordinates
 *
 */
class OpenGLPipeline
{
private:
    struct Internal;
    std::unique_ptr<Internal> m_internal;

    OpenGLPipeline();
protected:
public:
    struct ShaderStage
    {
        GLenum      shaderType;
        std::string shaderSource;

        ShaderStage(GLenum shaderType, std::string shaderSource) : shaderType(shaderType), shaderSource(shaderSource) {}
    };

    struct Uniform
    {
        GLenum      type;
        std::size_t dataSize;
        GLboolean   matrixTransponse;
        void       *data;
    };

    struct VertexAttributes
    {
        GLuint index;
    };

    struct DrawInfo
    {
        bool    instanced;
        GLenum  drawMode;
        GLsizei indiceCount;
        GLenum  indiceType;
        void   *indiceLocation;
    };

    struct RenderInfo
    {
        std::vector<GLuint>                    textures; /** Each texture in the vector is bound to texture units */
        std::vector<std::pair<GLenum, GLuint>> buffers;  /** Bind buffers targets to specified buffer objects */
        std::vector<std::pair<std::string, Uniform>> uniforms;
        // std::vector<std::pair<std::string, Attributes>> attributes;
        DrawInfo drawInfo;
    };

    OpenGLPipeline(const std::string &name, const core::graphics::Shader &shader);
    OpenGLPipeline(const std::string &name, const std::vector<ShaderStage> &shaderStages);
    ~OpenGLPipeline();

    OpenGLPipeline(OpenGLPipeline &o)            = delete;
    OpenGLPipeline &operator=(OpenGLPipeline &o) = delete;
    OpenGLPipeline(OpenGLPipeline &&o);
    OpenGLPipeline &operator=(OpenGLPipeline &&o);

    void render(const OpenGLMesh &mesh, const glm::mat4 &mvp) const;
    void render(const OpenGLMesh &mesh, const OpenGLTexture &texture, const glm::mat4 &mvp) const;
    void render(RenderInfo &renderInfo) const;

    /**
     * @brief Identify type of shader based from name
     *
     * @param name shader name
     * @return GLenum type of shader
     */
    static GLenum shaderTypeFromName(const std::string &name);

    /**
     * @brief Identify type of shader based from ShaderType
     *
     * @param name shader name
     * @return GLenum type of shader
     */
    static GLenum shaderTypeFromType(const core::graphics::Shader::ShaderType &shaderType);
};

/** @} endgroup OpenGL */
