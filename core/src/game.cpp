#include <core/game.hpp>

#include <core/ecs/entityManager.hpp>
#include <core/ecs/componentManager.hpp>
#include <core/ecs/components.hpp>

#include <core/event.hpp>
#include <core/audio/audioManager.hpp>
#include <core/input/inputManager.hpp>
#include <core/utils/profiler.hpp>
#include <core/utils/logging.hpp>
#include <core/utils/platform.hpp>

#include <core/assets/asset-inventory.hpp>

#include <core/graphics/window.hpp>
#include <core/graphics/asset-manager.hpp>
#include <core/graphics/renderer.hpp>

#if defined(CONFIG_CORE_RENDERER_VULKAN)
#include <core/graphics/renderer/vulkan/vk-renderer.hpp>
#endif

#if defined(CONFIG_CORE_RENDERER_OPENGL)
#include <core/graphics/renderer/opengl/gl-renderer.hpp>
#endif

#include <core/ui/uiManager.hpp>

#include <thread>

/** @todo: replace these with shared ptrs? */
Game                      *g_game             = nullptr;
core::audio::AudioManager *g_audioManager     = nullptr;
EntityManager             *g_entityManager    = nullptr;
ComponentManager          *g_componentManager = nullptr;
EventManager              *g_eventManager     = nullptr;
Renderer                  *g_renderer         = nullptr;
InputManager              *g_inputManager     = nullptr;
Time                      *g_time             = nullptr;
AssetManager              *g_assetManager     = nullptr;
AssetInventory            *g_assetInventory   = nullptr;
UIManager                 *g_uimanager        = nullptr;
core::graphics::Window    *g_window           = nullptr;

Game::Game(const std::string &windowTitle, const float &windowWidth, const float &windowHeight)
    : m_windowTitle(windowTitle),
      m_windowSize(windowWidth, windowHeight)
{
    /** Start profiling */
    PROFILER_START();

    L_TAG("Game::Game");

    // Change directory to project dir
    {
        L_TRACE("CWD: {}", core::utils::platform::getProjectPath().string());
        std::filesystem::current_path(core::utils::platform::getProjectPath());
    }

    /** @todo: thread-safe game instance creation for singleton */
    if (g_game) assert("Only one Game class object may exist");
    g_game = this;
    g_time = new Time();

    SDL_version sdlVersion;
    SDL_GetVersion(&sdlVersion);
    L_INFO("Using SDL2 Version: {}.{}.{}", sdlVersion.major, sdlVersion.minor, sdlVersion.patch);

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) L_THROW_RUNTIME("Could not initialize SDL2");

    L_DEBUG("Initializing AssetInventory");
    g_assetInventory = &AssetInventory::getInstance();
    L_DEBUG("Allocating AudioManager");
    g_audioManager = &core::audio::AudioManager::Instance();
    L_DEBUG("Initializing InputManager");
    g_inputManager = &InputManager::getInstance();
    L_DEBUG("Initializing EventManager");
    g_eventManager = new EventManager();
    L_DEBUG("Initializing ComponentManager");
    g_componentManager = &ComponentManager::getInstance();
    L_DEBUG("Initializing EntityManager");
    g_entityManager = &EntityManager::getInstance();
    L_DEBUG("Initializing UIManager");
    g_uimanager = &UIManager::getInstance();

#if defined(CONFIG_CORE_RENDERER_DEFAULT_OPENGL)
    L_DEBUG("Initializing Renderer (OpenGL)");
    g_window = new core::graphics::Window(
        std::make_unique<OpenGLRenderer>(OpenGLRenderer(windowTitle, windowWidth, windowHeight)));
    g_renderer     = &g_window->renderer();
    g_assetManager = &g_renderer->getAssetManager();

#elif defined(CONFIG_CORE_RENDERER_DEFAULT_VULKAN)
#error "Not yet supported, renderer interface needs work"
#endif

    /** FPS Defaults */
    m_targetDelta = time_ds(time_step::den / 60);
    L_DEBUG("Initialization done");
}

Game::~Game()
{
    delete g_entityManager;
    delete g_componentManager;
    delete g_eventManager;
    delete g_renderer;
    delete g_time;
    g_game = nullptr;

    PROFILER_END("dump.prof");
}

void Game::init()
{
    g_inputManager->init();
    g_eventManager->init();
    g_audioManager->init();
    g_uimanager->init();
}

void Game::startGameLoop()
{
    L_TAG("Game::startGameLoop");
    bool isRunning = true;

    /** Watch incoming events for exit triggers */
    SDL_AddEventWatch(
        [](void *data, SDL_Event *e) -> int {
            bool *isRunning = static_cast<bool *>(data);
            if (e->type == SDL_QUIT)
                *isRunning = false;
            else if (e->type == SDL_WINDOWEVENT && e->window.event == SDL_WINDOWEVENT_CLOSE)
                *isRunning = false;
            return 0; /** pass all events */
        },
        &isRunning);

    while (isRunning)
    {
        PROFILER_BLOCK("FRAME", profiler::colors::Red100);
        {
            PROFILER_BLOCK("Time::preupdate");
            g_time->preUpdate();
        }

        {
            /**
             * Manager::preupdate
             * Managers grab relevant SDL_events from queue here
             */
            PROFILER_BLOCK("Manager::preupdate");
            g_inputManager->preUpdate();
            g_entityManager->preUpdate();
        }
        SDL_PumpEvents();

        /** Manager Updates */
        {
            PROFILER_BLOCK("Manager::update");
            time_ms delta = g_time->scaledDeltaTime<time_ms>();
            g_inputManager->update(delta);
            g_entityManager->update(delta);
        }

        {
            PROFILER_BLOCK("Manager::postUpdate");
            g_entityManager->postUpdate();
            g_inputManager->postUpdate();
        }

        /** Render */
        {
            PROFILER_BLOCK("FrameRender");
            g_renderer->renderBegin();
            /**
             * Call render function from Renderer
             * This doesn't feel good tho, maybe rework?
             * @todo: maybe rework how the renderer gets components to be rendered (maybe scenes)
             */
            g_renderer->render();
            g_renderer->renderEnd();
        }

        /** Refresh manager objects */
        {
            PROFILER_BLOCK("Managers::Refresh");
            g_componentManager->refresh();
            g_entityManager->refresh();
        }

        {
            PROFILER_BLOCK("Time::postUpdate");
            g_time->postUpdate();
        }

        if (m_targetDelta > g_time->unscaledFrameTime())
        {
            PROFILER_BLOCK("FrameSleep");
            auto s = m_targetDelta - g_time->unscaledFrameTime();
            PROFILER_VALUE("FRAME_DELAY_US", s.count(), PROFILER_VIN("FRAME_DELAY_US"));
            std::this_thread::sleep_for(s);
            // SDL_Delay(std::chrono::duration_cast<std::chrono::milliseconds>(s).count());
        }

        {
            PROFILER_BLOCK("FPS Calculation");
            // logging::trace("{},{}: m_targetDelta:      ({})", __LINE__, __func__,
            // m_targetDelta.count()); logging::trace("{},{}: unscaledDeltaTime:  ({})", __LINE__,
            // __func__, g_time->unscaledDeltaTime().count()); logging::trace("{},{}:
            // unscaledFrameStart: ({})", __LINE__, __func__, g_time->unscaledFrameStart().count());
            // logging::trace("{},{}: unscaledFrameEnd:   ({})", __LINE__, __func__,
            // g_time->unscaledFrameEnd().count()); logging::trace("{},{}: unscaledFrameTime: ({})",
            // __LINE__, __func__, g_time->unscaledFrameTime().count()); logging::trace("{},{}:
            // scaledDeltaTime:    ({})", __LINE__, __func__, g_time->scaledDeltaTime().count());
            // logging::trace("{},{}: scaledFrameTime:    ({})", __LINE__, __func__,
            // g_time->scaledFrameTime().count()); logging::trace("{},{}: unscaledTime:       ({})",
            // __LINE__, __func__, g_time->unscaledTime().count());
            float fps = time_fs::period::den
                      / (Time::getTime<time_fs>() - g_time->unscaledFrameStart<time_fs>()).count();
            m_minfps = (fps < m_minfps ? fps : m_minfps);
            m_maxfps = (fps > m_maxfps ? fps : m_maxfps);
            m_fps    = fps;
            // logging::trace("{},{}: FPS: ({})", __LINE__, __func__, m_fps);
            // logging::trace("{},{}: MIN: ({})", __LINE__, __func__, m_minfps);
            PROFILER_VALUE("FPS", fps, PROFILER_VIN("FPS"));
        }
    }
}

void Game::setTargetFPS(const float fps)
{
    /** add one to adjust target */
    m_targetDelta = time_ds(static_cast<time_ds::rep>(time_step::den / fps));
}

glm::vec2 Game::getWindowSize() const noexcept { return this->m_windowSize; }

Game             *Game::this_game() { return g_game; }
EntityManager    *Game::entityManager() { return g_entityManager; }
ComponentManager *Game::componentManager() { return g_componentManager; }
EventManager     *Game::eventManager() { return g_eventManager; }
InputManager     *Game::inputManager() { return g_inputManager; }
Time             *Game::time() { return g_time; }
Renderer         *Game::renderer() { return g_renderer; }
AssetManager     *Game::assetManager() { return g_assetManager; }
AssetInventory   *Game::assetInventory() { return g_assetInventory; }