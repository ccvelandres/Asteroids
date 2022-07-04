#include <iostream>
#include <map>
#include <thread>
#include <typeindex>

#include <SDL2/SDL.h>

#include <ecs/ecs.hpp>
#include <event.hpp>
#include <game.hpp>
#include <utils/logging.hpp>

#include <enemy.hpp>
#include <player.hpp>

#include <renderer/vulkan/vk-renderer.hpp>

constexpr int windowWidth  = 1280;
constexpr int windowHeight = 720;
constexpr int originX      = windowWidth / 2;
constexpr int originY      = windowHeight / 2;

auto app_start = std::chrono::high_resolution_clock::now();

int main(int arc, char **argv)
{
    /** Set compact pattern for logging */
    logging::set_pattern("[%H:%M:%S.%e] [%t] %L : %v");
    logging::set_level(logging::level::trace);

    Game *game = new Game("Asteroids", windowWidth, windowHeight);
    game->init();
    game->setTargetFPS(90);

    // Player            player  = Game::entityManager()->addEntity<Player>();
    // EntityList<Enemy> enemies = Game::entityManager()->addEntities<Enemy>( 8 );

    // for ( int i = 0; i < enemies.size(); i++ )
    // {
    //     int    type = 0;
    //     Enemy &e    = *enemies [i].get();
    //     if ( i >= enemies.size() / 2 )
    //         type = 1;
    //     e.spawn( type, glm::vec3( ( windowWidth / enemies.size() ) * i, 50, 0 ) );
    // }

    game->startGameLoop();

    delete game;
    return 0;
}
