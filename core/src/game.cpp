#include <game.hpp>
#include <ecs/ecs.hpp>
#include <ecs/components.hpp>
#include <renderer.hpp>
#include <event.hpp>
#include <input/inputManager.hpp>

#include <thread>

Game *Game::m_game = nullptr;
EntityManager *Game::m_entityManager = nullptr;
ComponentManager *Game::m_componentManager = nullptr;
EventManager *Game::m_eventManager = nullptr;
Renderer *Game::m_renderer = nullptr;
InputManager *Game::m_inputManager = nullptr;
Time *Game::m_time = nullptr;

Game::Game(const std::string &windowTitle,
           const int &windowWidth,
           const int &windowHeight) : m_windowTitle(windowTitle),
                                      m_windowWidth(windowWidth),
                                      m_windowHeight(windowHeight)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        exit(1);
    }
    m_window = SDL_CreateWindow(m_windowTitle.c_str(),
                                SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED,
                                m_windowWidth,
                                m_windowHeight,
                                SDL_WINDOW_SHOWN);

    if (m_game)
        assert("Only one Game class object may exist");
    m_game = this;
    m_time = new Time();
    m_renderer = new Renderer();
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
}

void Game::init()
{
    m_inputManager->init();
    m_eventManager->init();
}

void Game::startGameLoop()
{
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
        m_time->preUpdate();

        /** Manager PreUpdates */

        /** Handle SDL Events here */
        {
            m_eventManager->handleEvents();
        }

        m_entityManager->preUpdate();

        /* Update inputs */
        m_inputManager->update();

        /** Manager Update */
        {
            time_ms delta = m_time->scaledDeltaTime<time_ms>();
            m_entityManager->update(delta);
            m_componentManager->update<TransformComponent>(delta);
            m_componentManager->update<SpriteComponent>(delta);
        }

        m_entityManager->postUpdate();
        m_inputManager->postUpdate();

        /** Render */
        {
            m_renderer->startRender();
            ComponentList<RenderComponent> rComponents = m_componentManager->getComponents<RenderComponent>();
            for (auto &c : rComponents)
            {
                if (c->enabled())
                    c->render();
            }
            m_renderer->endRender();
        }

        /** Refresh manager objects */
        m_componentManager->refresh();
        m_entityManager->refresh();

        m_time->postUpdate();
        if (m_targetDelta > m_time->unscaledFrameTime())
        {
            std::this_thread::sleep_for((m_targetDelta - m_time->unscaledFrameTime()));
        }

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
        m_fps.push(fps);
        logging::debug("{},{}: {} FPS: ({})", __LINE__, __func__, m_fps.index, m_fps.get());
        logging::trace("{},{}: MIN: ({})", __LINE__, __func__, m_minfps);
    }
}

void Game::setTargetFPS(const float fps)
{
    /** add one to adjust target */
    m_targetDelta = time_ds(static_cast<time_ds::rep>(time_step::den / fps));
}