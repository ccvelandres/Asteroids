#pragma once

/**
 * @file core/ecs/components/spriteRenderer.hpp
 * @author Cedric Velandres (ccvelandres@gmail.com)
 *
 * @addtogroup Components
 * @{
 */

#include "../component.hpp"
#include "renderComponent.hpp"
#include "../../graphics/renderer/renderer.hpp"

/**
 * @brief The SpriteRenderer allows rendering 2D sprites
 *
 */
class SpriteRenderer : public RenderComponent
{
private:
protected:
    SpriteRenderer();
    SpriteRenderer(AssetName texture);
    SpriteRenderer(AssetID meshID, AssetID textureID, AssetID pipelineID);
public:
    ~SpriteRenderer();
    SpriteRenderer(SpriteRenderer &o)             = delete;
    SpriteRenderer &operator=(SpriteRenderer &o)  = delete;
    SpriteRenderer(SpriteRenderer &&o)            = default;
    SpriteRenderer &operator=(SpriteRenderer &&o) = default;

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
