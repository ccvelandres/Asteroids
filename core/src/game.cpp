#include <game.hpp>
#include <ecs/ecs.hpp>
#include <ecs/components.hpp>
#include <renderer.hpp>
#include <renderer/vulkan/vk-renderer.hpp>
#include <event.hpp>
#include <input/inputManager.hpp>

#include <thread>

Game *Game::m_game = nullptr;
EntityManager *Game::m_entityManager = nullptr;
ComponentManager *Game::m_componentManager = nullptr;
EventManager *Game::m_eventManager = nullptr;
Renderer *Game::m_renderer = nullptr;
BaseRenderer *Game::m_baseRenderer = nullptr;
InputManager *Game::m_inputManager = nullptr;
Time *Game::m_time = nullptr;

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
    
    m_game = this;

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        exit(1);
    }
    m_window = SDL_CreateWindow(m_windowTitle.c_str(),
                                SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED,
                                m_windowWidth,
                                m_windowHeight,
                                SDL_WINDOW_SHOWN | SDL_WINDOW_VULKAN);

    if (m_window == NULL)
    {
        L_ERROR("Failed to initialize window");
    }

    if (m_game)
        assert("Only one Game class object may exist");
        
    m_time = new Time();
    m_baseRenderer = new VulkanRenderer(m_window);
    while(1);
    // m_renderer = new Renderer();
    m_inputManager = new InputManager();
    m_eventManager = new EventManager();
    m_componentManager = new ComponentManager();
    m_entityManager = new EntityManager();

    /** FPS Defaults */
    m_targetDelta = time_ds(time_step::den / 60);
}

Game::~Game()
{
    SDL_DestroyWindow(m_window);
    delete m_entityManager;
    delete m_componentManager;
    delete m_eventManager;
    delete m_renderer;
    delete m_time;
    m_game = nullptr;

    profiler::dumpBlocksToFile("dump.prof");
}

void Game::init()
{
    m_inputManager->init();
    m_eventManager->init();
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
            m_time->preUpdate();
        }

        /** Manager PreUpdates */

        /** Handle SDL Events here */
        {
            EASY_BLOCK("EventManager::handleEvents");
            m_eventManager->handleEvents();
        }

        {
            EASY_BLOCK("ECS::preupdate");
            m_entityManager->preUpdate();
        }

        /* Update inputs */
        {
            EASY_BLOCK("InputManager::update");
            m_inputManager->update();
        }

        /** Manager Update */
        {
            EASY_BLOCK("ECS::update");
            time_ms delta = m_time->scaledDeltaTime<time_ms>();
            m_entityManager->update(delta);
            m_componentManager->update<TransformComponent>(delta);
            m_componentManager->update<SpriteComponent>(delta);
        }

        {
            EASY_BLOCK("ECS::postupdate");
            m_entityManager->postUpdate();
            m_inputManager->postUpdate();
        }

        /** Render */
        {
            EASY_BLOCK("FrameRender");
            m_renderer->startRender();
            m_componentManager->foreach<RenderComponent>([](RenderComponent &c)
                                                         {
                if (c.enabled()) c.render(); });
            m_renderer->endRender();
        }

        /** Refresh manager objects */
        {
            EASY_BLOCK("Managers::Refresh");
            m_componentManager->refresh();
            m_entityManager->refresh();
        }

        {
            EASY_BLOCK("Time::postUpdate");
            m_time->postUpdate();
        }

        if (m_targetDelta > m_time->unscaledFrameTime())
        {
            EASY_BLOCK("FrameSleep");
            auto s = m_targetDelta - m_time->unscaledFrameTime();
            EASY_VALUE("FRAME_DELAY_US", s.count(), EASY_VIN("FRAME_DELAY_US"));
            std::this_thread::sleep_for(s);
            // SDL_Delay(std::chrono::duration_cast<std::chrono::milliseconds>(s).count());
        }

        {
            EASY_BLOCK("FPS Calculation");
            logging::trace("{},{}: m_targetDelta:      ({})", __LINE__, __func__, m_targetDelta.count());
            logging::trace("{},{}: unscaledDeltaTime:  ({})", __LINE__, __func__, m_time->unscaledDeltaTime().count());
            logging::trace("{},{}: unscaledFrameStart: ({})", __LINE__, __func__, m_time->unscaledFrameStart().count());
            logging::trace("{},{}: unscaledFrameEnd:   ({})", __LINE__, __func__, m_time->unscaledFrameEnd().count());
            logging::trace("{},{}: unscaledFrameTime:  ({})", __LINE__, __func__, m_time->unscaledFrameTime().count());
            logging::trace("{},{}: scaledDeltaTime:    ({})", __LINE__, __func__, m_time->scaledDeltaTime().count());
            logging::trace("{},{}: scaledFrameTime:    ({})", __LINE__, __func__, m_time->scaledFrameTime().count());
            logging::trace("{},{}: unscaledTime:       ({})", __LINE__, __func__, m_time->unscaledTime().count());
            float fps = time_fs::period::den / (Time::getTime<time_fs>() - m_time->unscaledFrameStart<time_fs>()).count();
            m_minfps = (fps < m_minfps ? fps : m_minfps);
            m_maxfps = (fps > m_maxfps ? fps : m_maxfps);
            m_fps = fps;
            logging::trace("{},{}: FPS: ({})", __LINE__, __func__, m_fps);
            logging::trace("{},{}: MIN: ({})", __LINE__, __func__, m_minfps);
            EASY_VALUE("FPS", fps, EASY_VIN("FPS"));
        }
    }
}

void Game::setTargetFPS(const float fps)
{
    /** add one to adjust target */
    m_targetDelta = time_ds(static_cast<time_ds::rep>(time_step::den / fps));
}