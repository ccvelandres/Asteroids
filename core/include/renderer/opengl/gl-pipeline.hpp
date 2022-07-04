#pragma once

#include <memory>
#include <vector>
#include <string>
#include "gl-wrapper.hpp"

#include <glm/glm.hpp>

#include "gl-mesh.hpp"

class OpenGLPipeline
{
private:
    struct Internal;
    std::unique_ptr<Internal> m_internal;
protected:
public:
    struct ShaderStage
    {
        GLenum      shaderType;
        std::string shaderSource;

        ShaderStage(GLenum shaderType, std::string shaderSource) : shaderType(shaderType), shaderSource(shaderSource) {}
    };

    OpenGLPipeline(const std::string &name, const std::vector<ShaderStage> &shaderStages);
    OpenGLPipeline(OpenGLPipeline &&o);
    OpenGLPipeline &operator=(OpenGLPipeline &&o);
    ~OpenGLPipeline();

    void render(const OpenGLMesh &mesh, const glm::mat4 &mvp) const;
};
