#pragma once

#include <cassert>
#include <memory>
#include <string>

#include "glm/glm.hpp"

#include "time.hpp"

class EntityManager;
class ComponentManager;
class EventManager;
class InputManager;
class Renderer;
class AssetManager;
class AssetInventory;

class Game
{
private:
    std::string m_windowTitle;
    glm::vec2 m_windowSize;

    time_ds m_targetDelta;
    float   m_minfps = std::numeric_limits<float>::max(), m_maxfps;
    float   m_fps;
protected:
public:
    Game(const std::string &windowTitle, const float &windowWidth, const float &windowHeight);

    ~Game();

    void init();
    void startGameLoop();

    void setTargetFPS(const float fps);

    glm::vec2 getWindowSize() const noexcept;

    /** @note Globals for manager objects (probably bad but this saves storing pointers on each object) */
    static Game             *this_game();        /** Get the current game instance */
    static EntityManager    *entityManager();    /** Get the EntityManager instance */
    static ComponentManager *componentManager(); /** Get the ComponentManager instance */
    static EventManager     *eventManager();     /** Get the EventManager instance */
    static InputManager     *inputManager();     /** Get the InputManager instance */
    static Time             *time();             /** Get the Time Manager instance */
    static Renderer         *renderer();         /** Get the Renderer instance */
    static AssetManager     *assetManager();     /** Get the Renderer's AssetManager instance */
    static AssetInventory   *assetInventory();   /** Get the AssetInventory instance */
};
