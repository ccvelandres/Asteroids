#include <core/assets/shader.hpp>
#include <core/utils/logging.hpp>
#include <core/assets/utils.hpp>

namespace core::assets
{
    int Shader::findShaderStage(const AssetPath &path)
    {
        L_TAG("Shader::findShaderStage");
        for (int index = 0; index < this->m_shaderStages.size(); ++index)
        {
            auto &stage = this->m_shaderStages[index];
            if (stage.path == path) return index;
        }

        return -1;
    }


    Shader::Shader(const std::string &shaderName) : m_shaderName(shaderName)
    {
        L_TAG("Shader::Shader");
        L_UNIMPLEMENTED("Shader::Shader");
    }

    Shader::Shader(const std::string                                           &shaderName,
                   const std::vector<std::pair<Shader::ShaderType, AssetPath>> &shaderStages)
        : m_shaderName(shaderName)
    {
        L_TAG("Shader::Shader");

        for (auto &stage : shaderStages)
        {
            this->addShaderStage(stage.first, stage.second);
            L_TRACE("Added shader stage: {}: {}", stage.first, stage.second);
        }
    }

    Shader::~Shader() = default;

    int Shader::addShaderStage(Shader::ShaderType shaderType, const AssetPath &path)
    {
        L_TAG("Shader::addShaderStage");
        int index = findShaderStage(path);

        if (index < 0)
        {
            ShaderStage shaderStage = {.path       = path,
                                       .shaderType = shaderType,
                                       .binaryData = core::assets::utils::loadBinaryFile(path)};
            index                   = this->m_shaderStages.size();
            this->m_shaderStages.push_back(std::move(shaderStage));
        }

        L_TRACE("Added shader stage: {}: {}", shaderType, path);
        return index;
    }

    const std::vector<Shader::ShaderStage> &Shader::getShaderStages() const noexcept
    {
        return this->m_shaderStages;
    }

    std::size_t       Shader::stageCount() const noexcept { return this->m_shaderStages.size(); }
    const std::string Shader::name() const noexcept { return this->m_shaderName; }

} // namespace core::assets
