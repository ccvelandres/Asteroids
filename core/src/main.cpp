#include <iostream>
#include <thread>
#include <map>

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

#include <game.hpp>
#include <event.hpp>
#include <ecs/ecs.hpp>

#include <logic/player.hpp>
#include <logic/enemy.hpp>

constexpr int windowWidth = 1280;
constexpr int windowHeight = 720;
constexpr int originX = windowWidth / 2;
constexpr int originY = windowHeight / 2;

auto app_start = std::chrono::high_resolution_clock::now();

static float time()
{
    using namespace std::chrono;
    auto ts = high_resolution_clock::now();
    // time_dns e = ts - te;
    float d = duration_cast<time_ds>(ts - app_start).count();
    // return duration_cast<duration<double, std::nano>>(e);
    return d;
}

int main(int arc, char **argv)
{

    Game *game = new Game("Asteroids", windowWidth, windowHeight);
    game->init();
    game->setTargetFPS(120);
    
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