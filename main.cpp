#include <iostream>
#define SDL_MAIN_HANDLED
#include "SDL.h"

#ifdef __cplusplus
extern "C"
#endif
int main()
{
    std::cout << "Hello world" << std::endl;
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        exit(1);
    }
    SDL_Window *window = SDL_CreateWindow("SDL2 Template",
                                          0, 0, 800, 600, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    bool isRunning = true;

    while (isRunning)
    {
        SDL_Event e;
        while (SDL_PollEvent(&e))
        {

            switch (e.type)
            {
            case SDL_WINDOWEVENT:
                if (e.window.event != SDL_WINDOWEVENT_CLOSE)
                    break;
            case SDL_QUIT:
                std::cout << "SDL_Quit event received" << std::endl;
                isRunning = false;
                break;
            default:
                break;
            }
        }

        // Clear screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, SDL_ALPHA_OPAQUE);
        SDL_RenderPresent(renderer);
        SDL_Delay(10);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    return 0;
}