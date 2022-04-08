#pragma once

#include <string>
#include <memory>
#include <cassert>

#include <SDL2/SDL.h>

class EntityManager;
class ComponentManager;
class EventManager;
class InputManager;
class Renderer;

class Game
{
private:
    SDL_Window *m_window;
    std::string m_windowTitle;
    int m_windowWidth, m_windowHeight;

    std::size_t m_frameStart, m_frameEnd, m_frameDelta, m_frameTime, m_targetDelta;
    float m_fps;

    static Game *m_game;
    static EntityManager *m_entityManager;
    static ComponentManager *m_componentManager;
    static Renderer *m_renderer;
    static EventManager *m_eventManager;
    static InputManager *m_inputManager;

    friend Renderer;

protected:
public:
    Game(const std::string &windowTitle,
         const int &windowWidth,
         const int &windowHeight);

    ~Game();

    /** @note Globals for manager objects (probably bad but this saves storing pointers on each object) */
    /** Get the current game object */
    static Game &this_game() { return *m_game; }
    /** Get the EntityManager Object */
    static EntityManager &entityManager() { return *m_entityManager; }
    /** Get the ComponentManager Object */
    static ComponentManager &componentManager() { return *m_componentManager; }
    /** Get the EventManager Object */
    static EventManager &eventManager() { return *m_eventManager; }
    /** Get the InputManager Object */
    static InputManager &inputManager() { return *m_inputManager; }
    
    /** Get the Renderer Object */
    static Renderer &renderer() { return *m_renderer; }

    void init();
    void startGameLoop();
};
