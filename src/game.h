#pragma once
#include "SDL.h"
#include "SDL_image.h"

class Game {
public:
    Game();
    ~Game();

    bool init(const char* title, int width, int height);
    void run();
    void close();

    SDL_Renderer* getRenderer()const;

private:
    SDL_Window* window;
    SDL_Renderer *renderer;
    bool isRunning;
};