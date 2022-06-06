#pragma once

#include <cassert>
#include <memory>
#include <string>

#include <SDL2/SDL.h>
#include <time.hpp>

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
    int         m_windowWidth, m_windowHeight;

    time_ds m_targetDelta;
    float   m_minfps = MAXFLOAT, m_maxfps;
    float   m_fps;

    friend Renderer;
protected:
public:
    Game( const std::string &windowTitle, const int &windowWidth, const int &windowHeight );

    ~Game();

    void init();
    void startGameLoop();

    void setTargetFPS( const float fps );

    /** @note Globals for manager objects (probably bad but this saves storing pointers on each object) */
    static Game             *this_game();        /** Get the current game object */
    static EntityManager    *entityManager();    /** Get the EntityManager Object */
    static ComponentManager *componentManager(); /** Get the ComponentManager Object */
    static EventManager     *eventManager();     /** Get the EventManager Object */
    static InputManager     *inputManager();     /** Get the InputManager Object */
    static Time             *time();             /** Get the Time Manager Object */
    static Renderer         *renderer();         /** Get the Renderer Object */
};
