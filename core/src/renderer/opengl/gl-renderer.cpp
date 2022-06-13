#include <renderer/opengl/gl-pipeline.hpp>
#include <renderer/opengl/gl-renderer.hpp>
#include <renderer/opengl/gl-wrapper.hpp>

#include <SDL2/SDL.h>

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

    // if (! glewIsSupported("GL_VERSION_4_6"))
    // {
    //     L_THROW_RUNTIME( "Required OpenGL extensions missing" );
    // }
}

SDL_Window *createWindow( const std::string &windowTitle, const int windowWidth, const int windowHeight )
{
    L_TAG( "createWindow" );

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

SDL_GLContext createContext( SDL_Window *window )
{
    L_TAG( "createContext" );

    SDL_GLContext context = SDL_GL_CreateContext( window );

    initGL();

    int viewportWidth, viewportHeight;
    SDL_GL_GetDrawableSize( window, &viewportWidth, &viewportHeight );

    L_TRACE( "OpenGL Context created with viewport size: {}, {}", viewportHeight, viewportWidth );

    glClearDepth( 1.0f );
    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_LEQUAL );
    glEnable( GL_CULL_FACE );
    glViewport( 0, 0, viewportWidth, viewportHeight );

    return context;
}

OpenGLPipeline createDefaultPipeline()
{
    std::vector<OpenGLPipeline::ShaderStage> stages;

    stages.push_back( OpenGLPipeline::ShaderStage( GL_VERTEX_SHADER, "shaders/opengl/default.vert" ) );
    stages.push_back( OpenGLPipeline::ShaderStage( GL_FRAGMENT_SHADER, "shaders/opengl/default.frag" ) );

    return OpenGLPipeline( stages );
}

struct OpenGLRenderer::Internal
{
    SDL_Window *const window;
    SDL_GLContext     context;

    OpenGLPipeline pipeline;

    Internal( const std::string &windowTitle, const int windowWidth, const int windowHeight )
        : window( ::createWindow( windowTitle, windowWidth, windowHeight ) ),
          context( ::createContext( window ) ),
          pipeline( ::createDefaultPipeline() )
    {
    }
};

OpenGLRenderer::OpenGLRenderer( const std::string &windowTitle, const int windowWidth, const int windowHeight )
    : m_internal( std::make_unique<Internal>( windowTitle, windowWidth, windowHeight ) )
{
}

OpenGLRenderer::OpenGLRenderer( OpenGLRenderer &&o )            = default;
OpenGLRenderer &OpenGLRenderer::operator=( OpenGLRenderer &&o ) = default;
OpenGLRenderer::~OpenGLRenderer()                               = default;

void OpenGLRenderer::init() {}

void OpenGLRenderer::update( const time_ms delta ) {}

void OpenGLRenderer::clean() {}

void OpenGLRenderer::refresh() {}

bool OpenGLRenderer::renderBegin() { return true; }

void OpenGLRenderer::render( const std::vector<assets::Mesh> &meshes ) {}

void OpenGLRenderer::renderEnd() {}
