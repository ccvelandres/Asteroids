#include <game.hpp>
#include <ecs/ecs.hpp>
#include <ecs/components.hpp>
#include <renderer/vulkan/vk-renderer.hpp>
#include <renderer/opengl/gl-renderer.hpp>
#include <event.hpp>
#include <input/inputManager.hpp>
#include <utils/profiler.hpp>
#include <utils/logging.hpp>

#include <thread>

Game *g_game = nullptr;
EntityManager *g_entityManager = nullptr;
ComponentManager *g_componentManager = nullptr;
EventManager *g_eventManager = nullptr;
Renderer *g_renderer = nullptr;
InputManager *g_inputManager = nullptr;
Time *g_time = nullptr;

Game::Game(const std::string &windowTitle,
           const int &windowWidth,
           const int &windowHeight) : m_windowTitle(windowTitle),
                                      m_windowWidth(windowWidth),
                                      m_windowHeight(windowHeight)
{
    L_TAG("Game::Game");

    /** Start profiling */
    EASY_PROFILER_ENABLE;
    profiler::startListen();
    
    if (g_game)
        assert("Only one Game class object may exist");
    g_game = this;
    g_time = new Time();

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        exit(1);
    }
    // m_window = SDL_CreateWindow(m_windowTitle.c_str(),
    //                             SDL_WINDOWPOS_CENTERED,
    //                             SDL_WINDOWPOS_CENTERED,
    //                             m_windowWidth,
    //                             m_windowHeight,
    //                             SDL_WINDOW_SHOWN | SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE);

    // if (m_window == NULL)
    // {
    //     L_ERROR("Failed to initialize window");
    // }
    // g_renderer = new VulkanRenderer(m_window);
    g_renderer = new OpenGLRenderer(windowTitle, windowWidth, windowHeight);

    g_inputManager = new InputManager();
    g_eventManager = new EventManager();
    g_componentManager = new ComponentManager();
    g_entityManager = new EntityManager();
    
    /** FPS Defaults */
    m_targetDelta = time_ds(time_step::den / 60);
}

Game::~Game()
{
    SDL_DestroyWindow(m_window);
    delete g_entityManager;
    delete g_componentManager;
    delete g_eventManager;
    delete g_renderer;
    delete g_time;
    g_game = nullptr;

    profiler::dumpBlocksToFile("dump.prof");
}

void Game::init()
{
    g_inputManager->init();
    g_eventManager->init();
}

void Game::startGameLoop()
{
    L_TAG("Game::startGameLoop");
    bool isRunning = true;

    /** Register Event filter for exit @todo: replace this */
    SDL_AddEventWatch([](void *data, SDL_Event *e) -> int
                      {
        bool *isRunning = static_cast<bool*>(data);
        switch (e->type) {
            case SDL_WINDOWEVENT:
                if (e->window.event != SDL_WINDOWEVENT_CLOSE)
                    break;
            case SDL_QUIT:
                *isRunning = false;
                break;
            default: break;
        }
        return 0; /** ignored */ },
                      &isRunning);

    while (isRunning)
    {
        EASY_BLOCK("FRAME", profiler::colors::Red100);
        {
            EASY_BLOCK("Time::preupdate");
            g_time->preUpdate();
        }

        /** Manager PreUpdates */

        /** Handle SDL Events here */
        {
            EASY_BLOCK("EventManager::handleEvents");
            g_eventManager->handleEvents();
        }

        {
            EASY_BLOCK("ECS::preupdate");
            g_entityManager->preUpdate();
        }

        /* Update inputs */
        {
            EASY_BLOCK("InputManager::update");
            g_inputManager->update();
        }

        /** Manager Update */
        {
            EASY_BLOCK("ECS::update");
            time_ms delta = g_time->scaledDeltaTime<time_ms>();
            g_entityManager->update(delta);
            g_componentManager->update<TransformComponent>(delta);
        }

        {
            EASY_BLOCK("ECS::postupdate");
            g_entityManager->postUpdate();
            g_inputManager->postUpdate();
        }

        /** Render */
        {
            EASY_BLOCK("FrameRender");
            g_renderer->renderBegin();
            /** @todo: Maybe rework this part to let the scene or renderer handle 
             * the draw calls
             */
            g_renderer->renderEnd();
        }

        /** Refresh manager objects */
        {
            EASY_BLOCK("Managers::Refresh");
            g_componentManager->refresh();
            g_entityManager->refresh();
        }

        {
            EASY_BLOCK("Time::postUpdate");
            g_time->postUpdate();
        }

        if (m_targetDelta > g_time->unscaledFrameTime())
        {
            EASY_BLOCK("FrameSleep");
            auto s = m_targetDelta - g_time->unscaledFrameTime();
            EASY_VALUE("FRAME_DELAY_US", s.count(), EASY_VIN("FRAME_DELAY_US"));
            std::this_thread::sleep_for(s);
            // SDL_Delay(std::chrono::duration_cast<std::chrono::milliseconds>(s).count());
        }

        {
            EASY_BLOCK("FPS Calculation");
            // logging::trace("{},{}: m_targetDelta:      ({})", __LINE__, __func__, m_targetDelta.count());
            // logging::trace("{},{}: unscaledDeltaTime:  ({})", __LINE__, __func__, g_time->unscaledDeltaTime().count());
            // logging::trace("{},{}: unscaledFrameStart: ({})", __LINE__, __func__, g_time->unscaledFrameStart().count());
            // logging::trace("{},{}: unscaledFrameEnd:   ({})", __LINE__, __func__, g_time->unscaledFrameEnd().count());
            // logging::trace("{},{}: unscaledFrameTime:  ({})", __LINE__, __func__, g_time->unscaledFrameTime().count());
            // logging::trace("{},{}: scaledDeltaTime:    ({})", __LINE__, __func__, g_time->scaledDeltaTime().count());
            // logging::trace("{},{}: scaledFrameTime:    ({})", __LINE__, __func__, g_time->scaledFrameTime().count());
            // logging::trace("{},{}: unscaledTime:       ({})", __LINE__, __func__, g_time->unscaledTime().count());
            float fps = time_fs::period::den / (Time::getTime<time_fs>() - g_time->unscaledFrameStart<time_fs>()).count();
            m_minfps = (fps < m_minfps ? fps : m_minfps);
            m_maxfps = (fps > m_maxfps ? fps : m_maxfps);
            m_fps = fps;
            // logging::trace("{},{}: FPS: ({})", __LINE__, __func__, m_fps);
            // logging::trace("{},{}: MIN: ({})", __LINE__, __func__, m_minfps);
            EASY_VALUE("FPS", fps, EASY_VIN("FPS"));
        }
    }
}

void Game::setTargetFPS(const float fps)
{
    /** add one to adjust target */
    m_targetDelta = time_ds(static_cast<time_ds::rep>(time_step::den / fps));
}

Game *Game::this_game() { return g_game; }
EntityManager *Game::entityManager() { return g_entityManager; }
ComponentManager *Game::componentManager() { return g_componentManager; }
EventManager *Game::eventManager() { return g_eventManager; }
InputManager *Game::inputManager() { return g_inputManager; }
Time *Game::time() { return g_time; }
Renderer *Game::renderer() { return g_renderer; }