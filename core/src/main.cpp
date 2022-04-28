#include <iostream>
#include <thread>
#include <map>
#include <typeindex>

#include <SDL.h>
#include <SDL2_gfxPrimitives.h>

#include <game.hpp>
#include <event.hpp>
#include <ecs/ecs.hpp>
#include <utils/logging.hpp>

#include <logic/player.hpp>
#include <logic/enemy.hpp>

constexpr int windowWidth = 1280;
constexpr int windowHeight = 720;
constexpr int originX = windowWidth / 2;
constexpr int originY = windowHeight / 2;

auto app_start = std::chrono::high_resolution_clock::now();

int main(int arc, char **argv)
{
    logging::set_level(logging::level::off);

    Game *game = new Game("Asteroids", windowWidth, windowHeight);
    game->init();
    game->setTargetFPS(90);
    
    Player player = Game::entityManager().addEntity<Player>();
    EntityList<Enemy> enemies = Game::entityManager().addEntities<Enemy>(8);

    for (int i = 0; i < enemies.size(); i++)
    {
        int type=0;
        Enemy &e = *enemies[i].get();
        if (i < enemies.size() / 2) {
            
        }
        else {
            type = 1;
        }
        e.spawn(type, Vector3F((windowWidth / enemies.size()) * i, 50, 0));
    }

    game->startGameLoop();

    return 0;
}