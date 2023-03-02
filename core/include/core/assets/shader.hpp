#pragma once

/**
 * @file core/assets/shader.hpp
 * @author Cedric Velandres (ccvelandres@gmail.com)
 *
 * @addtogroup Assets
 * @{
 */

#include "asset-inventory.hpp"

#include <memory>

namespace core::assets
{

    /**
     * @brief Base class for Shader stage
     *
     */
    class Shader
    {
    public:
        enum class ShaderType
        {
            Unknown,
            Vertex,
            Fragment,
            Geometry,
            Compute,
            TesselationCtrl,
            TesselationEval,
            RayGeneration,
            RayIntersection,
            RayAnyHit,
            RayClosestHit,
            RayMiss,
            RayCallable,
            Mesh,
            Task
        };

        struct ShaderStage
        {
            AssetPath          path;
            Shader::ShaderType shaderType;
            std::vector<char>  binaryData;
        };

    private:
        std::string              m_shaderName;
        std::vector<ShaderStage> m_shaderStages;

        /**
         * @brief Find the index of the shader
         *
         * @param path path to shader source
         * @return int index of shader in stage, otherwise -1
         */
        int findShaderStage(const AssetPath &path);

    protected:
    public:
        Shader(const std::string &shaderName);
        Shader(const std::string                                           &shaderName,
               const std::vector<std::pair<Shader::ShaderType, AssetPath>> &shaderStages);
        ~Shader();

        Shader(Shader &o)             = delete;
        Shader &operator=(Shader &o)  = delete;
        Shader(Shader &&o)            = default;
        Shader &operator=(Shader &&o) = default;

        int addShaderStage(Shader::ShaderType shaderType, const AssetPath &name);

        /** @brief Returns reference to shader stage vector */
        const std::vector<ShaderStage> &getShaderStages() const noexcept;
        /** @brief Returns number of shader stages */
        std::size_t stageCount() const noexcept;
        /** @brief Returns the shader name */
        const std::string name() const noexcept;

        /** @brief Identify shader with name */
        static ShaderType getTypeFromExt(const AssetName &name);
    };

} // namespace core::assets

/** @} endgroup Renderer */