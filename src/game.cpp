#include "game.h"
#include "controls.h"
#include "character.h"
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>



Game::Game() : window(nullptr), renderer(nullptr), isRunning(false) {}

Game::~Game() {
    close();
}

bool Game::init(const char *title, int width, int height) {
    if(SDL_Init(SDL_INIT_VIDEO) <0){
        std::cerr << "SDL Init failed: " << SDL_GetError() << std::endl;
        return false;
    }
    window = SDL_CreateWindow(title,SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              width, height, SDL_WINDOW_SHOWN);
    if(!window){
        std::cerr << "window failed: " << SDL_GetError() << std::endl;
        return false;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(!renderer){
        std::cerr << "window creation failed: " << SDL_GetError() <<std::endl;
        return false;
    }
    if(!(IMG_Init(IMG_INIT_PNG)& IMG_INIT_PNG)){
        std::cerr<<" img init failed : " << IMG_GetError()<<std::endl;
        return false;}
    isRunning = true;
    return true;
}
void Game::run() {
    SDL_Event event;
    Character player(renderer, "../assets/textures/testPlayer.png", 100, 100);
    Controls controls;

    while (isRunning) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) isRunning = false;
            controls.handleInput(event, player);
        }

    //SDL_SetRenderDrawColor(renderer,255,204,221,255);
    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);

    SDL_RenderClear(renderer);
    player.draw();

    SDL_RenderPresent(renderer);

}
}

void Game::close() {
    if(renderer) SDL_DestroyRenderer(renderer);
    if(window) SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
}
SDL_Renderer* Game::getRenderer() const {
    return renderer;
}