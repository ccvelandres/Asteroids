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

    SDL_GLContext context = SDL_GL_CreateContext(window);

    initGL();

    /** Set Vsync */
    L_ASSERT(SDL_GL_SetSwapInterval(1) == 0, "Could not set Vsync");

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

    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_CULL_FACE);
    glViewport(0, 0, viewportWidth, viewportHeight);

    return context;
}

struct OpenGLRenderer::Internal
{
    SDL_Window *const  window;
    SDL_GLContext      context;
    OpenGLAssetManager assetManager;

    Internal(const std::string &windowTitle, const int windowWidth, const int windowHeight)
        : window(::createWindow(windowTitle, windowWidth, windowHeight)),
          context(::createContext(window)),
          assetManager()
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

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    return true;
}

#include <core/ecs/componentManager.hpp>
#include <core/ecs/components.hpp>
#include <core/ecs/entity.hpp>
#include <glm/gtx/transform.hpp>
#include <core/graphics/camera/camera.hpp>
#include <core/game.hpp>

void OpenGLRenderer::render()
{
    /** @todo: this needs reworking, better rendering management for other types */
    ComponentManager   &componentManager = ComponentManager::getInstance();
    OpenGLAssetManager &am               = dynamic_cast<OpenGLAssetManager &>(this->getAssetManager());
    Camera             *camera           = Game::camera();
    OpenGLPipeline     &pipeline         = m_internal->assetManager.getPipeline(0);

    ComponentList<MeshRenderer> meshComponents = componentManager.getComponents<MeshRenderer>();

    glm::mat4 projectionMatrix = camera->getProjectionMatrix();
    glm::mat4 viewMatrix       = camera->getViewMatrix();

    for (auto &it : meshComponents)
    {
        OpenGLMesh         &mesh        = am.getMesh(it->getMeshID());
        OpenGLTexture      &texture     = am.getTexture(it->getTextureID());
        TransformComponent &transform   = it->entity().getComponent<TransformComponent>();
        glm::mat4           modelMatrix = it->getModelMatrix();
        glm::mat4           mvp(projectionMatrix * viewMatrix * modelMatrix);

        pipeline.render(mesh, mvp);
    }
}

void OpenGLRenderer::renderEnd() { SDL_GL_SwapWindow(m_internal->window); }

AssetManager &OpenGLRenderer::getAssetManager() { return m_internal->assetManager; }