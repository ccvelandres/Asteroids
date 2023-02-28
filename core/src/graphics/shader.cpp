#include <core/graphics/shader.hpp>
#include <core/utils/logging.hpp>
#include <core/assets/utils.hpp>

namespace core::graphics
{


    int Shader::findShaderStage(const AssetPath &path)
    {
        L_TAG("Shader::Internal::findShaderStage");
        for (int index = 0; index < this->m_shaderStages.size(); ++index)
        {
            auto &stage = this->m_shaderStages[index];
            if (stage.path == path) return index;
        }

        return -1;
    }

    int Shader::addStage(Shader::ShaderType type, const AssetPath &path)
    {
        L_TAG("Shader::Internal::addStage");
        int index = findShaderStage(path);

        if(index < 0)
        {
            ShaderStage shaderStage = {
                .path = path,
                .shaderType = type,
                .binaryData = core::assets::utils::loadBinaryFile(path)
            };
            index = this->m_shaderStages.size();
            this->m_shaderStages.push_back(std::move(shaderStage));
        }

        return index;
    }

    Shader::Shader()  = default;
    Shader::~Shader() = default;

    Shader::Shader(const std::vector<std::pair<ShaderType, AssetPath>> &shaderStages)
    { 
        L_TAG("Shader::Shader");

        for( auto &stage : shaderStages)
        {
            this->addStage(stage.first, stage.second);
            L_TRACE("Added shader stage: {}: {}", stage.first, stage.second);
        }
     }

    int Shader::addShaderStage(Shader::ShaderType shaderType, const AssetPath &name)
    {
        L_TAG("Shader::addShaderStage");
        
        this->addStage(shaderType, name);
        L_TRACE("Added shader stage: {}: {}", shaderType, name);
    }


} // namespace core::graphics
