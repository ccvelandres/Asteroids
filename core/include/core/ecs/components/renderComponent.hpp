#pragma once

/**
 * @file core/ecs/components/renderComponent.hpp
 * @author Cedric Velandres (ccvelandres@gmail.com)
 *
 * @addtogroup Components
 * @{
 */

#include "../component.hpp"
#include "transformComponent.hpp"
#include "../../graphics/renderer/renderer.hpp"

#include <bitset>

constexpr std::size_t maxRenderMaskBits = 32;
constexpr std::size_t defaultRenderMask = 0x0001;
using RenderMask                        = std::bitset<maxRenderMaskBits>;

enum RenderMaskBits
{
    NONE    = 0x0000,
    DEFAULT = 0x0001, // Default render mask for renderComponents
    TEXT    = 0x0002, // For text render targets
    WINDOW  = 0x0004, // For window frames
    GUI     = 0x0006  // GUI windows (TEXT | WINDOW)
};

/**
 * @brief The RenderComponent allows an entity to be rendered to the screen
 *
 */
class RenderComponent : public Component
{
private:
protected:
    /** Protected Constructors (use entity to add components) */
    RenderComponent();
    RenderComponent(AssetID meshID, AssetID textureID, AssetID pipelineID)
        : m_meshID(meshID),
          m_textureID(textureID),
          m_pipelineID(pipelineID)
    {
    }
public:
    RenderMask m_renderMask{defaultRenderMask};
    glm::mat4  m_modelMatrix;
    AssetID    m_meshID;
    AssetID    m_textureID;
    AssetID    m_pipelineID;

    ~RenderComponent();
    RenderComponent(RenderComponent &o)             = delete;
    RenderComponent &operator=(RenderComponent &o)  = delete;
    RenderComponent(RenderComponent &&o)            = default;
    RenderComponent &operator=(RenderComponent &&o) = default;

    /** Component overrides */
    // void awake() override;
    // void init() override;
    // void fixedUpdate(time_ms delta) override;
    // void update(time_ms delta) override;
    // void clean() override;

    void        setRenderMask(RenderMask renderMask) { m_renderMask = renderMask; }
    RenderMask &getRenderMask() { return m_renderMask; };

    void setMeshID(const AssetID id) noexcept { m_meshID = id; }
    void setTextureID(const AssetID id) noexcept { m_textureID = id; }
    void setPipelineID(const AssetID id) noexcept { m_pipelineID = id; }

    AssetID getMeshID() const noexcept { return m_meshID; }
    AssetID getTextureID() const noexcept { return m_textureID; }
    AssetID getPipelineID() const noexcept { return m_pipelineID; }

    /** Retrieve the attached entity's model matrix (translation * rotation * scale) */
    const glm::mat4 getModelMatrix() noexcept;

    friend Entity;
    friend EntityManager;
    friend ComponentManager;
};

/** @} endgroup Components */
