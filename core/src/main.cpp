#include <iostream>
#include <thread>
#include <map>

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

#include <game.hpp>
#include <event.hpp>
#include <ecs/ecs.hpp>

#include <logic/player.hpp>

constexpr int windowWidth = 800;
constexpr int windowHeight = 600;
constexpr int originX = windowWidth / 2;
constexpr int originY = windowHeight / 2;

int main(int arc, char **argv)
{
    Game *game = new Game("Asteroids", windowWidth, windowHeight);

    // Player player;

    // {
    //     Texture test = Game::renderer().loadTexture("assets/clouds.png");
    //     {
    //         Texture t(test);
    //         std::cout << "2 usecount" << std::endl;
    //     }
    // }
    game->init();

    Player player = Game::entityManager().addEntity<Player>();
    
    game->startGameLoop();
    // if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    // {
    //     std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
    //     exit(1);
    // }
    // SDL_Window *window = SDL_CreateWindow("SDL2 Template", 0, 0, windowWidth, windowHeight, SDL_WINDOW_SHOWN);
    // SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // bool isRunning = true;

    // while (isRunning)
    // {
    //     EASY_BLOCK("gameLoop", profiler::colors::Red);
    //     Vector2F mouse;
    //     Vector2F input;
    //     const float velocity = 1;

    //     SDL_Event e;
    //     int evCount = 0;
    //     std::map<std::size_t, std::vector<SDL_Event>> evMap;
    //     while (SDL_PollEvent(&e))
    //     {
    //         evCount++;
    //         evMap[e.type].push_back(e);

    //         switch (e.type)
    //         {
    //         case SDL_WINDOWEVENT:
    //             if (e.window.event != SDL_WINDOWEVENT_CLOSE)
    //                 break;
    //         case SDL_QUIT:
    //             std::cout << "SDL_Quit event received" << std::endl;
    //             isRunning = false;
    //             break;
    //         case SDL_MOUSEMOTION:
    //             mouse.x = e.motion.x;
    //             mouse.y = e.motion.y;
    //             break;
    //         case SDL_KEYDOWN:
    //             if (e.key.keysym.sym == SDLK_UP)
    //                 input.y = -velocity;
    //             else if (e.key.keysym.sym == SDLK_DOWN)
    //                 input.y = velocity;
    //             if (e.key.keysym.sym == SDLK_LEFT)
    //                 input.x = -velocity;
    //             else if (e.key.keysym.sym == SDLK_RIGHT)
    //                 input.x = velocity;
    //             break;
    //         case SDL_KEYUP:
    //             if (e.key.keysym.sym == SDLK_UP)
    //                 input.y = 0;
    //             else if (e.key.keysym.sym == SDLK_DOWN)
    //                 input.y = 0;
    //             if (e.key.keysym.sym == SDLK_LEFT)
    //                 input.x = 0;
    //             else if (e.key.keysym.sym == SDLK_RIGHT)
    //                 input.x = 0;
    //             break;
    //         default:
    //             break;
    //         }
    //     }

    //     for (auto &it : evMap) {
    //         std::cout << "EVCode: " << std::hex << it.first << ": " << it.second.size() << std::endl;

    //     }

    //     // Clear screen
    //     SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    //     SDL_RenderClear(renderer);

    //     SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, SDL_ALPHA_OPAQUE);
    //     SDL_RenderPresent(renderer);
    //     SDL_Delay(1);
    // }

    // SDL_DestroyRenderer(renderer);
    // SDL_DestroyWindow(window);

    return 0;
}