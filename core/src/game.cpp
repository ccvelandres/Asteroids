#include <game.hpp>
#include <ecs/ecs.hpp>
#include <ecs/components.hpp>
#include <renderer.hpp>
#include <event.hpp>
#include <input/inputManager.hpp>

Game *Game::m_game = nullptr;
EntityManager *Game::m_entityManager = nullptr;
ComponentManager *Game::m_componentManager = nullptr;
EventManager *Game::m_eventManager = nullptr;
Renderer *Game::m_renderer = nullptr;
InputManager *Game::m_inputManager = nullptr;

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
    m_renderer = new Renderer();
    m_inputManager = new InputManager();
    m_eventManager = new EventManager();
    m_componentManager = new ComponentManager();
    m_entityManager = new EntityManager();

    /** FPS Defaults */
    m_targetDelta = 1000 / 60;
}

Game::~Game()
{
    SDL_DestroyWindow(m_window);
    delete m_entityManager;
    delete m_componentManager;
    delete m_eventManager;
    delete m_renderer;
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

    while (isRunning)
    {
        m_frameStart = SDL_GetTicks();
        m_frameDelta = m_frameStart - m_frameEnd;
        std::cout << "FPS  : " << m_fps << std::endl;
        std::cout << "Start: " << m_frameStart << std::endl;
        std::cout << "End  : " << m_frameEnd << std::endl;
        std::cout << "Delta: " << m_frameDelta << std::endl;

        /** Manager PreUpdates */

        /** Handle SDL Events here */
        {
            m_eventManager->handleEvents();
        }

        /* Update inputs */
        m_inputManager->update();

        /** Manager Update */
        m_entityManager->updateEntities(m_frameDelta);
        m_componentManager->update<TransformComponent>(m_frameDelta);
        m_componentManager->update<SpriteComponent>(m_frameDelta);

        /** Render */
        {
            m_renderer->startRender();
            ComponentList<RenderComponent> rComponents = m_componentManager->getComponents<RenderComponent>();
            for (auto &c : rComponents) {
                c->render();
            }
            m_renderer->endRender();
        }

        /** Refresh manager objects */
        m_componentManager->refresh();
        m_entityManager->refresh();

        m_inputManager->postUpdate();

        m_frameEnd = SDL_GetTicks();
        m_frameTime = m_frameEnd - m_frameStart;
        if (m_targetDelta > m_frameTime)
        {
            SDL_Delay(m_targetDelta - m_frameTime);
        }
        m_fps = 1000.0f / (static_cast<float>(SDL_GetTicks()) - static_cast<float>(m_frameStart));
    }
}