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
#include "../../assets/spriteSheet.hpp"

#include <memory>

/**
 * @brief The SpriteRenderer allows rendering 2D sprites
 *
 */
class SpriteRenderer : public RenderComponent
{
private:
    bool                                       usingSpriteSheet = false;
    std::shared_ptr<core::assets::SpriteSheet> spriteSheet;
    std::size_t                                activeFrameNumber;
    core::assets::SpriteSheet::Frame           activeFrame;
protected:
    SpriteRenderer();
    SpriteRenderer(AssetID meshID, AssetID textureID, AssetID pipelineID);
public:
    ~SpriteRenderer();
    SpriteRenderer(SpriteRenderer &o)             = delete;
    SpriteRenderer &operator=(SpriteRenderer &o)  = delete;
    SpriteRenderer(SpriteRenderer &&o)            = default;
    SpriteRenderer &operator=(SpriteRenderer &&o) = default;

    void loadSprite(const AssetName &sprite);
    void loadSpriteSheet(const AssetName &sprite);
    void setSpriteTag(const std::string &tagName, std::size_t frameNumber = 0);

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
