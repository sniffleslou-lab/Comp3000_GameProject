//main.cpp -> window opening and testing
//using sdl2

#include <SDL.h>
#include <iostream>

int main(int argc, char* args[]){
    SDL_Window* window = NULL;
    SDL_Surface* screenSurface = NULL;
    if (SDL_Init(SDL_INIT_VIDEO)<0)
    {
        std::cerr<< "sdl not installed correctly SDL_ERROR: " << SDL_GetError() << std::endl;
        return 1;
    }
    window = SDL_CreateWindow("Am I pretty enough?", SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              800,600, SDL_WINDOW_SHOWN);
    if (window == NULL){
        std::cerr<< "window could not be created! sdl_error: " << SDL_GetError << std::endl;
        SDL_Quit();
        return 1;
    }

    screenSurface = SDL_GetWindowSurface(window);
    SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface ->format, 0xFF, 0xCC, 0xDD));
    SDL_UpdateWindowSurface(window);

    SDL_Delay(2000);
    SDL_DestroyWindow(window);
    return 0;
}

