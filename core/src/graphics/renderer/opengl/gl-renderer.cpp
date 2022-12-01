#include <core/graphics/renderer/opengl/gl-pipeline.hpp>
#include <core/graphics/renderer/opengl/gl-renderer.hpp>
#include <core/graphics/renderer/opengl/gl-wrapper.hpp>
#include <core/graphics/renderer/opengl/gl-assetManager.hpp>

#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>

#include <utils/logging.hpp>

static void initGL()
{
    L_TAG("initGL");

    const char *version = (const char *)(glGetString(GL_VERSION));
    if (version != NULL) L_DEBUG("OpenGL Version: {}", version);
    const char *vendor = (const char *)(glGetString(GL_VENDOR));
    if (vendor != NULL) L_INFO("OpenGL Vendor: {}", vendor);
    const char *renderer = (const char *)(glGetString(GL_RENDERER));
    if (renderer != NULL) L_DEBUG("OpenGL Renderer: {}", renderer);

    /** Initialize glew */
    GLenum gres = glewInit();
    if (gres != GLEW_OK)
    {
        L_THROW_RUNTIME("Failed to initialize glew");
    }
}

SDL_Window *createWindow(const std::string &windowTitle, const int windowWidth, const int windowHeight)
{
    L_TAG("createWindow");

    L_DEBUG("SDL VideoDriver: {}", SDL_GetCurrentVideoDriver());

    SDL_Window *window = SDL_CreateWindow(windowTitle.c_str(),
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          windowWidth,
                                          windowHeight,
                                          SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);

    if (window == NULL)
    {
        L_THROW_RUNTIME("Could not create window");
    }

    return window;
}

SDL_GLContext createContext(SDL_Window *window)
{
    L_TAG("createContext");

    int           iRes    = 0;
    SDL_GLContext context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, context);

    initGL();

    /** Set Vsync */
    iRes = SDL_GL_SetSwapInterval(1);
    L_TRACE("Vsync {}", iRes == 0 ? "disabled" : "enabled");

    int viewportWidth, viewportHeight;
    SDL_GL_GetDrawableSize(window, &viewportWidth, &viewportHeight);
    L_TRACE("OpenGL Context created with viewport size: {}, {}", viewportHeight, viewportWidth);

    GLenum error;
    // Initialize Projection Matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    error = glGetError();
    L_ASSERT(error == GL_NO_ERROR,
             "Error initializing Projection Matrix: {}",
             reinterpret_cast<const char *>(glewGetErrorString(error)));

    // Initialize Modelview Matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    error = glGetError();
    L_ASSERT(error == GL_NO_ERROR,
             "Error initializing Modelview Matrix: {}",
             reinterpret_cast<const char *>(glewGetErrorString(error)));

    // Enable Alpha blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Enable Depth testing (clear first) with LEQUAL function test
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    // Enable face culling, defaults to GL_BACK (back-facing faces)
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    // Set viewport size

    glViewport(0, 0, viewportWidth, viewportHeight);

    return context;
}

struct OpenGLRenderer::Internal
{
    SDL_Window *const  window;
    SDL_GLContext      context;
    OpenGLAssetManager assetManager;
    AssetID            defaultPipeline;

    Internal(const std::string &windowTitle, const int windowWidth, const int windowHeight)
        : window(::createWindow(windowTitle, windowWidth, windowHeight)),
          context(::createContext(window)),
          assetManager(),
          defaultPipeline(assetManager.loadAsset(AssetType::Pipeline, "default"))
    {
        L_TAG("OpenGLRenderer::Internal");
        L_TRACE("Internal resources initialized ({})", static_cast<void *>(this));
    }

    ~Internal()
    {
        L_TAG("OpenGLRenderer::~Internal");
        L_TRACE("Internal resources freed ({})", static_cast<void *>(this));
    }
};

OpenGLRenderer::OpenGLRenderer(const std::string &windowTitle, const int windowWidth, const int windowHeight)
    : m_internal(std::make_unique<Internal>(windowTitle, windowWidth, windowHeight))
{
}

OpenGLRenderer::OpenGLRenderer(OpenGLRenderer &&o)            = default;
OpenGLRenderer &OpenGLRenderer::operator=(OpenGLRenderer &&o) = default;
OpenGLRenderer::~OpenGLRenderer()                             = default;

void OpenGLRenderer::init() {}

void OpenGLRenderer::update(const time_ms delta) {}

void OpenGLRenderer::clean() {}

void OpenGLRenderer::refresh() {}

bool OpenGLRenderer::renderBegin()
{
    // SDL_GL_MakeCurrent(m_internal->window, m_internal->context);

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    return true;
}

#include <core/ecs/componentManager.hpp>
#include <core/ecs/components.hpp>
#include <core/ecs/entity.hpp>
#include <glm/gtx/transform.hpp>
#include <core/game.hpp>

static void render(OpenGLAssetManager                  &am,
                   std::shared_ptr<CameraComponent>    &camera,
                   const ComponentList<SpriteRenderer> &components)
{
    glm::mat4 projectionMatrix = camera->getProjectionMatrix();
    glm::mat4 viewMatrix       = camera->getViewMatrix();

    for (auto &sprite : components)
    {
        // if &'ed masks does not match renderComponents' mask, skip
        if (sprite->m_renderMask.none() || (sprite->m_renderMask & camera->getRenderMask()) != sprite->m_renderMask)
            continue;

        OpenGLMesh     &mesh     = am.getMesh(sprite->getMeshID());
        OpenGLTexture  &texture  = am.getTexture(sprite->getTextureID());
        OpenGLPipeline &pipeline = am.getPipeline(sprite->getPipelineID());

        glm::mat4 modelMatrix = sprite->getModelMatrix();
        glm::mat4 mvp         = projectionMatrix * viewMatrix * modelMatrix;

        glm::vec4 tile(0.0f, 0.0f, 0.2f, 0.5f);

        OpenGLPipeline::RenderInfo renderInfo;
        renderInfo.textures = {texture.getTextureID()};
        renderInfo.buffers  = {
             {GL_ARRAY_BUFFER,         mesh.getVertexBufferID()},
             {GL_ELEMENT_ARRAY_BUFFER, mesh.getIndiceBufferID()}
        };
        renderInfo.uniforms = {
            {"u_mvp",  {GL_FLOAT_MAT4, sizeof(mvp), GL_FALSE, static_cast<void *>(&mvp[0][0])}},
            {"u_tile", {GL_FLOAT_VEC4, sizeof(tile), GL_FALSE, static_cast<void *>(&tile)}    }
        };
        renderInfo.drawInfo = {.instanced      = false,
                               .drawMode       = GL_TRIANGLES,
                               .indiceCount    = static_cast<GLsizei>(mesh.getIndiceCount()),
                               .indiceType     = GL_UNSIGNED_INT,
                               .indiceLocation = static_cast<void *>(0)};

        pipeline.render(renderInfo);
    }
}

static void render(OpenGLAssetManager                &am,
                   std::shared_ptr<CameraComponent>  &camera,
                   const ComponentList<MeshRenderer> &components)
{
    glm::mat4 projectionMatrix = camera->getProjectionMatrix();
    glm::mat4 viewMatrix       = camera->getViewMatrix();

    for (auto &meshR : components)
    {
        // if &'ed masks does not match renderComponents' mask, skip
        if (meshR->m_renderMask.none() || (meshR->m_renderMask & camera->getRenderMask()) != meshR->m_renderMask)
            continue;

        OpenGLMesh     &mesh     = am.getMesh(meshR->getMeshID());
        OpenGLTexture  &texture  = am.getTexture(meshR->getTextureID());
        OpenGLPipeline &pipeline = am.getPipeline(meshR->getPipelineID());

        glm::mat4 modelMatrix = meshR->getModelMatrix();
        glm::mat4 mvp         = projectionMatrix * viewMatrix * modelMatrix;

        pipeline.render(mesh, texture, mvp);
    }
}

void OpenGLRenderer::render()
{
    L_TAG("OpenGLRenderer::render");

    /** @todo: this needs reworking, better rendering management for other types */
    ComponentManager                &componentManager = ComponentManager::getInstance();
    OpenGLAssetManager              &am               = dynamic_cast<OpenGLAssetManager &>(this->getAssetManager());

    /** @todo: fix depth rendering for multiple cameras */
    for(auto camera : componentManager.getComponents<CameraComponent>())
    {
        ::render(am, camera, componentManager.getComponents<SpriteRenderer>());
        ::render(am, camera, componentManager.getComponents<MeshRenderer>());
    }

}

void OpenGLRenderer::renderEnd() { SDL_GL_SwapWindow(m_internal->window); }

AssetManager &OpenGLRenderer::getAssetManager() { return m_internal->assetManager; }