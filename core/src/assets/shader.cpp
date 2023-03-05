#include <core/assets/shader.hpp>
#include <core/utils/logging.hpp>
#include <core/assets/utils.hpp>

namespace core::assets
{
    Shader::ShaderType Shader::getTypeFromExt(const AssetName &name)
    {
        L_TAG("Shader::getTypeFromExt");
        ShaderType type;

        const std::unordered_map<std::string, ShaderType> shaderExtensions = {
            {".vert",  ShaderType::Vertex         }, // Vertex Shader
            {".vs",    ShaderType::Vertex         }, // Vertex Shader
            {".frag",  ShaderType::Fragment       }, // Fragment Shader
            {".fs",    ShaderType::Fragment       }, // Fragment Shader
            {".gs",    ShaderType::Geometry       }, // Geometry Shader
            {".geom",  ShaderType::Geometry       }, // Geometry Shader
            {".comp",  ShaderType::Compute        }, // Compute Shader
            {".tesc",  ShaderType::TesselationCtrl}, // Tesselation Control Shader
            {".tese",  ShaderType::TesselationEval}, // Tesselation Evaluation Shader
            {".rgen",  ShaderType::RayGeneration  }, // Ray Generation shader
            {".rint",  ShaderType::RayIntersection}, // Ray Intersection shader
            {".rahit", ShaderType::RayAnyHit      }, // Ray Any-hit shader
            {".rchit", ShaderType::RayClosestHit  }, // Ray Closest-hit shader
            {".rmiss", ShaderType::RayMiss        }, // Ray Miss shader
            {".rcall", ShaderType::RayCallable    }, // Ray Callable shader
            {".mesh",  ShaderType::Mesh           }, // Mesh Shader
            {".task",  ShaderType::Task           }  // Task Shader
        };

        auto it = shaderExtensions.find(name);
        if( it != shaderExtensions.end())
            return it->second;
        else
            L_THROW_RUNTIME("Could not identify the shader type: {}", name);
    }

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
        const AssetPaths &assetPaths =
            AssetInventory::getInstance().lookupAssets(AssetType::Pipeline, shaderName);

        L_ASSERT(assetPaths.size() > 0, "Found zero paths for {}", shaderName);
        L_DEBUG("Loading shader \"{}\" with {} stages", shaderName, assetPaths.size());

        for (const auto &stage : assetPaths)
        {
            std::string shaderExt = stage.substr(stage.find_last_of('.'));
            ShaderType shaderType = Shader::getTypeFromExt(shaderExt);
            this->addShaderStage(shaderType, stage);
        }
    }

    Shader::Shader(const std::string                                           &shaderName,
                   const std::vector<std::pair<Shader::ShaderType, AssetPath>> &shaderStages)
        : m_shaderName(shaderName)
    {
        L_TAG("Shader::Shader");

        for (auto &stage : shaderStages)
        {
            this->addShaderStage(stage.first, stage.second);
            L_TRACE("Added shader stage: {}: {}", static_cast<int>(stage.first), stage.second);
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

        L_TRACE("Added shader stage: {}: {}", static_cast<int>(shaderType), path);
        return index;
    }

    const std::vector<Shader::ShaderStage> &Shader::getShaderStages() const noexcept
    {
        return this->m_shaderStages;
    }

    std::size_t       Shader::stageCount() const noexcept { return this->m_shaderStages.size(); }
    const std::string Shader::name() const noexcept { return this->m_shaderName; }

} // namespace core::assets
