#pragma once

/**
 * @file core/ecs/components/meshRenderer.hpp
 * @author Cedric Velandres (ccvelandres@gmail.com)
 *
 * @addtogroup Components
 * @{
 */

#include "../component.hpp"
#include "renderComponent.hpp"

/**
 * @brief The MeshRenderer allows rendering of mesh based render targets
 *
 */
class MeshRenderer : public RenderComponent
{
private:
protected:
    MeshRenderer(AssetID meshID, AssetID textureID) : RenderComponent(meshID, textureID, 0) {}
    MeshRenderer(AssetID meshID, AssetID textureID, AssetID pipelineID) : RenderComponent(meshID, textureID, pipelineID)
    {
    }
public:

    /** Component overrides */
    // void awake() override;
    // void init() override
    // void update(time_ms delta) override;
    // void clean() override;


    friend Entity;
    friend EntityManager;
    friend ComponentManager;
};

/** @} endgroup Components */
