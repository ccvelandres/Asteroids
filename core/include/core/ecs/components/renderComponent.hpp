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

/**
 * @brief The RenderComponent allows an entity to be rendered to the screen
 *
 */
class RenderComponent : public Component
{
private:
protected:
    AssetID m_meshID;
    AssetID m_textureID;
    AssetID m_pipelineID;

    TransformComponent *m_transform;

    /** Protected Constructors (use entity to add components) */
    RenderComponent();
    RenderComponent(AssetID meshID, AssetID textureID, AssetID pipelineID)
        : m_meshID(meshID),
          m_textureID(textureID),
          m_pipelineID(pipelineID)
    {
    }
    
public:
    ~RenderComponent();
    RenderComponent(RenderComponent &o)             = delete;
    RenderComponent &operator=(RenderComponent &o)  = delete;
    RenderComponent(RenderComponent &&o)            = default;
    RenderComponent &operator=(RenderComponent &&o) = default;

    /** Component overrides */
    // void awake() override;
    void init() override;
    // void fixedUpdate(time_ms delta) override;
    // void update(time_ms delta) override;
    // void clean() override;

    AssetID getMeshID() const { return m_meshID; }
    AssetID getTextureID() const { return m_textureID; }
    AssetID getPipelineID() const { return m_pipelineID; }

    virtual void getModelMatrix() = 0;

    friend Entity;
    friend EntityManager;
    friend ComponentManager;
};

/** @} endgroup Components */
