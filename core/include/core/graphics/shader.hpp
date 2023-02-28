#pragma once

/**
 * @file core/renderer/shader.hpp
 * @author Cedric Velandres (ccvelandres@gmail.com)
 *
 * @addtogroup Graphics
 * @{
 */

#include "../assets/asset-inventory.hpp"

#include <memory>

namespace core::graphics
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
            TesselationControl,
            TesselationEvaluation,
            RayGeneration,
            RayAnyHit,
            RayClosestHit,
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
        std::vector<ShaderStage> m_shaderStages;

        /**
         * @brief Find the index of the shader
         * 
         * @param path path to shader source
         * @return int index of shader in stage, otherwise -1
         */
        int findShaderStage(const AssetPath &path);

        /**
         * @brief Add a shader stage
         * 
         * @param type type of shader stage
         * @param path path to shader asset
         * @return int index of shader in stage
         */
        int addStage(Shader::ShaderType type, const AssetPath &path);
    protected:
    public:
        Shader(const std::vector<std::pair<ShaderType, AssetPath>> &shaderStages);
        Shader();
        ~Shader();

        Shader(Shader &o)             = delete;
        Shader &operator=(Shader &o)  = delete;
        Shader(Shader &&o)            = default;
        Shader &operator=(Shader &&o) = default;

        int addShaderStage(ShaderType shaderType, const AssetPath &name);

        /** @brief Returns reference to shader stage vector */
        const std::vector<ShaderStage> &getShaderStages() const noexcept;
        /** @brief Returns number of shader stages */
        std::size_t stageCount() const noexcept;
    };

} // namespace core::graphics

/** @} endgroup Renderer */