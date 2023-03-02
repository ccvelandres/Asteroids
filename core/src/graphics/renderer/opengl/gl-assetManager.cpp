#include <core/graphics/renderer/opengl/gl-assetManager.hpp>
#include <utils/logging.hpp>

#include <unordered_map>
#include <mutex>

struct OpenGLAssetManager::Internal
{
    template <typename T>
    using AssetCache = std::vector<std::pair<std::string, T>>;

    /** @todo: improve mutex & thread-safe impl */
    /** @todo: probably better to replace this with vector? but need to ensure index-value should not change ever */
    std::unordered_map<AssetType, std::unordered_map<AssetName, AssetID>> assetCache;
    std::vector<OpenGLMesh>                                               meshCache;
    std::vector<OpenGLPipeline>                                           pipelineCache;
    std::vector<OpenGLTexture>                                            textureCache;
    AssetCache<OpenGLMesh>                                                t_meshCache;
    AssetCache<OpenGLPipeline>                                            shaderCache;
    AssetCache<OpenGLTexture>                                             t_textureCache;
    std::mutex                                                            mutex;

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

    template <typename T>
    bool findCache(const std::string &assetName, const AssetCache<T> &cache, AssetID &id)
    {
        std::lock_guard<std::mutex> l(this->mutex);

        for (int index = 0; index < cache.size(); ++index)
        {
            auto &entry = cache.at(index);
            if (assetName == entry.first)
            {
                id = index;
                return true;
            }
        }
        return false;
    }

    AssetID loadPipeline(const core::assets::Shader &shader)
    {
        L_TAG("OpenGLAssetManager::loadPipeline");
        std::lock_guard<std::mutex> l(this->mutex);
        const std::string          &name      = shader.name();
        auto                       &cache     = this->shaderCache;
        auto                        insertPos = cache.end();
        AssetID                     id        = cache.size();

        cache.insert(insertPos, std::make_pair(name, OpenGLPipeline(shader)));
        L_DEBUG("Pipeline created {}: {}", id, name);

        return id;
    }

    AssetID loadMesh(const core::assets::Mesh &mesh)
    {
        L_TAG("OpenGLAssetManager::loadMesh");
        std::lock_guard<std::mutex> l(this->mutex);
        const std::string          &name      = mesh.name();
        auto                       &cache     = this->t_meshCache;
        auto                        insertPos = cache.end();
        AssetID                     id        = cache.size();

        cache.insert(insertPos, std::make_pair(name, OpenGLMesh(mesh)));
        L_DEBUG("Mesh created {}: {}", id, name);

        return id;
    }

    AssetID loadTexture(const core::assets::Texture &texture)
    {
        L_TAG("OpenGLAssetManager::loadTexture");
        std::lock_guard<std::mutex> l(this->mutex);
        const std::string          &name      = texture.name();
        auto                       &cache     = this->t_textureCache;
        auto                        insertPos = cache.end();
        AssetID                     id        = cache.size();

        cache.insert(insertPos, std::make_pair(name, OpenGLTexture(texture)));
        L_DEBUG("Texture created {}: {}", id, name);

        return id;
    }
};

static AssetID loadPipeline(const AssetName                        &name,
                            std::vector<OpenGLPipeline>            &cache,
                            std::unordered_map<AssetName, AssetID> &assetIDCache)
{
    L_TAG("OpenGLAssetManager::loadPipeline");
    constexpr AssetType type       = AssetType::Pipeline;
    auto                insertPos  = cache.end();
    AssetID             id         = cache.size();
    AssetName           shaderName = AssetName("opengl/" + name);

    /** Check if the asset is in cache */
    auto idCache = assetIDCache.find(shaderName);
    if (idCache != assetIDCache.end())
    {
        /** Check that asset id is within the cache */
        if (idCache->second < id)
        {
            L_DEBUG("Pipeline ({}) loaded from cache: {}", shaderName, idCache->second);
            return idCache->second;
        }
    }

    /** Get paths from asset manager */
    AssetPaths assetPaths = AssetInventory::getInstance().resolvePath(type, shaderName);

    L_DEBUG("Loading shader pipeline \"{}\" with {} stages", name, assetPaths.size());
    std::vector<OpenGLPipeline::ShaderStage> stages;

    /** Need to indentify type of shader based from name */
    for (auto &stagePath : assetPaths)
    {
        std::string shaderExt = stagePath.substr(stagePath.find_last_of("."));
        GLenum      type      = OpenGLPipeline::shaderTypeFromName(shaderExt);
        stages.push_back(OpenGLPipeline::ShaderStage(type, stagePath));
    }

    cache.insert(insertPos, OpenGLPipeline(shaderName, stages));
    assetIDCache[shaderName] = id; // add asset to cache
    L_DEBUG("Pipeline created {}: {}", id, shaderName);
    return id;
}

static AssetID loadMesh(const AssetName                        &name,
                        std::vector<OpenGLMesh>                &cache,
                        std::unordered_map<AssetName, AssetID> &assetIDCache)
{
    L_TAG("OpenGLAssetManager::loadMesh");
    constexpr AssetType type      = AssetType::Mesh;
    auto                insertPos = cache.end();
    AssetID             id        = cache.size();

    /** Check if the asset is in cache */
    auto idCache = assetIDCache.find(name);
    if (idCache != assetIDCache.end())
    {
        /** Check that asset id is within the cache */
        if (idCache->second < id)
        {
            L_DEBUG("Mesh ({}) loaded from cache: {}", name, idCache->second);
            return idCache->second;
        }
    }

    /** Get paths from asset manager */
    L_DEBUG("Loading mesh from {}", name);
    AssetPaths assetPaths = AssetInventory::getInstance().resolvePath(type, name);

    /** Parse mesh file */
    assert(assetPaths.size() == 1);
    // core::assets::Mesh mesh(assetPaths[0]);
    core::assets::Mesh mesh;
    cache.insert(insertPos, OpenGLMesh(mesh));
    assetIDCache[name] = id; // add asset to cache
    L_DEBUG("Mesh loaded {}: {}", id, name);
    return id;
}
static AssetID loadTexture(const AssetName                        &name,
                           std::vector<OpenGLTexture>             &cache,
                           std::unordered_map<AssetName, AssetID> &assetIDCache)
{
    L_TAG("OpenGLAssetManager::loadMesh");
    constexpr AssetType type      = AssetType::Texture;
    auto                insertPos = cache.end();
    AssetID             id        = cache.size();

    /** Check if the asset is in cache */
    auto idCache = assetIDCache.find(name);
    if (idCache != assetIDCache.end())
    {
        /** Check that asset id is within the cache */
        if (idCache->second < id)
        {
            L_DEBUG("Mesh ({}) loaded from cache: {}", name, idCache->second);
            return idCache->second;
        }
    }

    /** Get paths from asset manager */
    AssetPaths assetPaths = AssetInventory::getInstance().resolvePath(type, name);

    /** Load texture from file */
    assert(assetPaths.size() == 1);
    core::assets::Texture texture(assetPaths[0]);

    cache.insert(insertPos, OpenGLTexture(texture));
    assetIDCache[name] = id; // add asset to cache
    L_DEBUG("Texture loaded {}: {}", id, name);
    return id;
}

AssetID OpenGLAssetManager::loadAsset(const AssetType &type, const AssetName &name)
{
    AssetID id = std::numeric_limits<std::size_t>::max();
    switch (type)
    {
    case AssetType::Mesh:
        if (!this->m_internal->findCache(name, this->m_internal->t_meshCache, id))
            id = this->loadMesh(core::assets::Mesh(name));
        break;
    case AssetType::Pipeline:
        if (!this->m_internal->findCache(name, this->m_internal->shaderCache, id))
            id = this->loadPipeline(core::assets::Shader(name));
        break;
    case AssetType::Texture:
        if (!this->m_internal->findCache(name, this->m_internal->t_textureCache, id))
            id = this->loadTexture(core::assets::Texture(name));
        break;
    default:
        break;
    }
    return id;
}

AssetID OpenGLAssetManager::loadMesh(const core::assets::Mesh &mesh)
{
    L_TAG("OpenGLAssetManager::loadMesh");
    AssetID id;

    if (!this->m_internal->findCache(mesh.name(), this->m_internal->t_meshCache, id))
    {
        id = this->m_internal->loadMesh(mesh);
    }

    return id;
}

AssetID OpenGLAssetManager::loadTexture(const core::assets::Texture &texture)
{
    L_TAG("OpenGLAssetManager::loadTexture");
    AssetID id;

    if (!this->m_internal->findCache(texture.name(), this->m_internal->t_textureCache, id))
    {
        id = this->m_internal->loadTexture(texture);
    }

    return id;
}

AssetID OpenGLAssetManager::loadPipeline(const core::assets::Shader &shader)
{
    L_TAG("OpenGLAssetManager::loadPipeline");
    AssetID id;

    if (!this->m_internal->findCache(shader.name(), this->m_internal->shaderCache, id))
    {
        id = this->m_internal->loadPipeline(shader);
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