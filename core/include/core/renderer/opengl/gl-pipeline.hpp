#pragma once

/**
 * @file gl-pipeline.hpp
 * @author Cedric Velandres (ccvelandres@gmail.com)
 */

/**
 * @addtogroup OpenGL
 * @ingroup Renderer
 * @{
 */

#include <memory>
#include <vector>
#include <string>
#include "gl-wrapper.hpp"

#include <glm/glm.hpp>

#include "gl-mesh.hpp"
#include "gl-texture.hpp"

/**
 * @brief OpenGL Rendering pipeline
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

    OpenGLPipeline(const std::string &name, const std::vector<ShaderStage> &shaderStages);
    ~OpenGLPipeline();
    
    OpenGLPipeline(OpenGLPipeline &o)            = delete;
    OpenGLPipeline &operator=(OpenGLPipeline &o) = delete;
    OpenGLPipeline(OpenGLPipeline &&o);
    OpenGLPipeline &operator=(OpenGLPipeline &&o);

    void render(const OpenGLMesh &mesh, const glm::mat4 &mvp) const;
    void render(const OpenGLMesh &mesh, const OpenGLTexture &texture, const glm::mat4 &mvp) const;

    /**
     * @brief Identify type of shader based from name
     *
     * @param name shader name
     * @return GLenum type of shader
     */
    static GLenum shaderTypeFromName(const std::string &name);
};

/** @} endgroup OpenGL */
