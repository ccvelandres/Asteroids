#include <core/ecs/components/spriteRenderer.hpp>

#include <core/game.hpp>
#include <core/utils/logging.hpp>
#include <core/graphics/asset-manager.hpp>

#include <mutex>

static std::mutex spriteCacheMutex;
/** @todo: provide generic caching for shared objects */
static std::unordered_map<AssetName, std::shared_ptr<core::assets::SpriteSheet>> spriteSheetCache;

static std::shared_ptr<core::assets::SpriteSheet> loadSpriteSheet(const AssetName &sprite)
{
    std::lock_guard<std::mutex> l(spriteCacheMutex);
    auto                       &spriteSheet = spriteSheetCache[sprite];
    if (!spriteSheet)
    {
        spriteSheet.reset(new core::assets::SpriteSheet(sprite));
    }

    return spriteSheet;
}

SpriteRenderer::SpriteRenderer()
{
    AssetManager &assetManager = *Game::assetManager();
    AssetID       pipelineID   = assetManager.loadAsset(AssetType::Pipeline, "sprite");
    AssetID       meshID       = assetManager.loadAsset(AssetType::Mesh, "defaults/quad");

    m_meshID     = meshID;
    m_pipelineID = pipelineID;
}
SpriteRenderer::~SpriteRenderer() = default;

SpriteRenderer::SpriteRenderer(AssetID meshID, AssetID textureID, AssetID pipelineID)
    : RenderComponent(meshID, textureID, pipelineID)
{
}

void SpriteRenderer::loadSprite(const AssetName &sprite)
{
    AssetManager &assetManager = *Game::assetManager();
    AssetID       textureID    = assetManager.loadAsset(AssetType::Texture, sprite);

    m_textureID = textureID;
}
void SpriteRenderer::loadSpriteSheet(const AssetName &sprite)
{
    AssetManager &assetManager = *Game::assetManager();

    // load spritesheet metadata
    spriteSheet = ::loadSpriteSheet(sprite);

    m_textureID      = assetManager.loadAsset(AssetType::Texture, spriteSheet->getTextureName());
    usingSpriteSheet = true;
    activeFrameNumber      = 0;
}

void SpriteRenderer::setSpriteTag(const std::string &tagName, std::size_t frameNumber)
{
    L_TAG("SpriteRenderer::setSpriteTag()");
    L_ASSERT(spriteSheet, "component does not have a spritesheet");

    auto &spriteData = spriteSheet->getSpriteData();
    auto  spriteTag  = spriteData.tags.find(tagName);
    if (spriteTag != spriteData.tags.end())
    {
        core::assets::SpriteSheet::Tag tag = spriteTag->second;
        activeFrameNumber = tag.from + frameNumber;
    }
    else
    {
        L_TRACE("Spritesheet has no tag named: {}", spriteData.sheetName, tagName);
    }
}