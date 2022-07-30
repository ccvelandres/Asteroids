#include <core/ecs/components/spriteRenderer.hpp>
#include <core/game.hpp>
#include <core/graphics/asset-manager.hpp>

SpriteRenderer::SpriteRenderer()  = default;
SpriteRenderer::~SpriteRenderer() = default;

SpriteRenderer::SpriteRenderer(AssetName texture)  {
    AssetManager &assetManager = *Game::assetManager();
    AssetID textureID = assetManager.loadAsset(AssetType::Texture, texture);
    AssetID meshID = assetManager.loadAsset(AssetType::Mesh, "crate");//"defaults/quad");
    AssetID pipelineID = 0;

    m_textureID = textureID;
    m_meshID = meshID;
    m_pipelineID = pipelineID;
    m_renderMask = 0x1;
}

SpriteRenderer::SpriteRenderer(AssetID meshID, AssetID textureID, AssetID pipelineID)
    : RenderComponent(meshID, textureID, pipelineID)
{
}