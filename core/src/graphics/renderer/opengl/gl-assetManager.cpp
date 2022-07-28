#include <core/graphics/renderer/opengl/gl-assetManager.hpp>
#include <utils/logging.hpp>

#include <unordered_map>

struct OpenGLAssetManager::Internal
{
    /** @todo: probably better to replace this with vector? but need to ensure index-value should not change ever */
    std::vector<OpenGLMesh>     meshCache;
    std::vector<OpenGLPipeline> pipelineCache;
    std::vector<OpenGLTexture>  textureCache;

    Internal()
    {
        L_TAG("OpenGLAssetManager::Internal");
        L_TRACE("Internal resources initialized ({})", static_cast<void *>(this));
    }

    ~Internal()
    {
        L_TAG("OpenGLAssetManager::~Internal");
        L_TRACE("Internal resources freed ({})", static_cast<void *>(this));
    }
};

static AssetID loadPipeline(const std::string &name,std::vector<OpenGLPipeline> &cache)
{
    L_TAG("OpenGLAssetManager::loadPipeline");
    constexpr AssetType type = AssetType::Pipeline;
    auto    insertPos = cache.end();
    AssetID id        = cache.size();

    /** Get paths from asset manager */
    AssetPaths assetPaths = AssetInventory::getInstance().resolvePath(type, name);

    L_DEBUG("Loading shader pipeline \"{}\" with {} stages", name, assetPaths.size());
    std::vector<OpenGLPipeline::ShaderStage> stages;

    /** Need to indentify type of shader based from name */
    for (auto &stagePath : assetPaths)
    {
        std::string shaderExt = stagePath.substr(stagePath.find_last_of("."));
        GLenum      type      = OpenGLPipeline::shaderTypeFromName(shaderExt);
        stages.push_back(OpenGLPipeline::ShaderStage(type, stagePath));
    }

    cache.insert(insertPos, OpenGLPipeline(name, stages));
    L_DEBUG("Pipeline created {}: {}", id, name);
    return id;
}

static AssetID loadMesh(const std::string &name, std::vector<OpenGLMesh>     &cache)
{
    L_TAG("OpenGLAssetManager::loadMesh");
    constexpr AssetType type = AssetType::Mesh;
    auto    insertPos = cache.end();
    AssetID id        = cache.size();

    /** Get paths from asset manager */
    L_DEBUG("Loading mesh from {}", name);
    AssetPaths assetPaths = AssetInventory::getInstance().resolvePath(type, name);

    /** Parse mesh file */
    assert(assetPaths.size() == 1);
    assets::Mesh mesh(assetPaths[0]);

    cache.insert(insertPos, OpenGLMesh(mesh));
    L_DEBUG("Mesh loaded {}: {}", id, name);
    return id;
}
static AssetID loadTexture(const std::string &name, std::vector<OpenGLTexture>  &cache)
{
    L_TAG("OpenGLAssetManager::loadMesh");
    constexpr AssetType type = AssetType::Texture;
    auto    insertPos = cache.end();
    AssetID id        = cache.size();

    /** Get paths from asset manager */
    AssetPaths assetPaths = AssetInventory::getInstance().resolvePath(type, name);

    /** Load texture from file */
    assert(assetPaths.size() == 1);
    assets::Texture texture(assetPaths[0]);

    cache.insert(insertPos, OpenGLTexture(texture));
    L_DEBUG("Texture loaded {}: {}", id, name);

    return id;
}

AssetID OpenGLAssetManager::loadAsset(const AssetType &type, const AssetName &name)
{
    AssetID id = std::numeric_limits<std::size_t>::max();
    switch (type)
    {
    case AssetType::Mesh:
        id = loadMesh(name, m_internal->meshCache);
        break;
    case AssetType::Pipeline:
        id = loadPipeline(name, m_internal->pipelineCache);
        break;
    case AssetType::Texture:
        id = loadTexture(name, m_internal->textureCache);
        break;
    default:
        break;
    }
    return id;
}

OpenGLMesh &OpenGLAssetManager::getMesh(AssetID id) const
{
    L_TAG("OpenGLAssetManager::getMesh");
    auto &cache = m_internal->meshCache;
    if (id < cache.size())
        return cache.at(id);
    else
        L_THROW_RUNTIME("Cache empty for AssetID: {}", id);
}

OpenGLPipeline &OpenGLAssetManager::getPipeline(AssetID id) const
{
    L_TAG("OpenGLAssetManager::getPipeline");
    auto &cache = m_internal->pipelineCache;
    if (id < cache.size())
        return cache.at(id);
    else
        L_THROW_RUNTIME("Cache empty for AssetID: {}", id);
}

OpenGLTexture &OpenGLAssetManager::getTexture(AssetID id) const
{
    L_TAG("OpenGLAssetManager::getTexture");
    auto &cache = m_internal->textureCache;
    if (id < cache.size())
        return cache.at(id);
    else
        L_THROW_RUNTIME("Cache empty for AssetID: {}", id);
}

OpenGLAssetManager::OpenGLAssetManager() : m_internal(std::make_unique<Internal>()) {}
OpenGLAssetManager::OpenGLAssetManager(OpenGLAssetManager &&o)            = default;
OpenGLAssetManager &OpenGLAssetManager::operator=(OpenGLAssetManager &&o) = default;
OpenGLAssetManager::~OpenGLAssetManager()                                 = default;