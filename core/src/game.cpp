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
    m_targetDelta = time_ds(1.0f / 60);
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
        m_frameStart = Time::time();
        m_frameDelta = m_frameStart - m_frameEnd;
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
        m_entityManager->update(m_frameDelta);
        m_componentManager->update<TransformComponent>(m_frameDelta);
        m_componentManager->update<SpriteComponent>(m_frameDelta);

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

        m_frameEnd = Time::time();
        m_frameTime = m_frameEnd - m_frameStart;
        if (m_targetDelta > m_frameTime)
        {
            std::this_thread::sleep_for(time_ds(m_targetDelta - m_frameTime));
        }
        m_fps = 1.f / (Time::time() - m_frameStart).count();
        m_minfps = (m_fps < m_minfps ? m_fps : m_minfps);
        m_maxfps = (m_fps > m_maxfps ? m_fps : m_maxfps);
        std::cout << "FPS: " << m_fps << std::endl;
        std::cout << "MIN: " << m_minfps << std::endl;
        std::cout << "FTS: " << m_frameTime.count() << std::endl;
    }
}

void Game::setTargetFPS(const float fps)
{
    /** add one to adjust target */
    m_targetDelta = time_ds(1.0f / (fps + 1.0f));
}