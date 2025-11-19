//
// Created by Lou halpin on 18/11/2025.
//
#include "StartScene.h"
#include <SDL_image.h>
#include <iostream>

StartScene::StartScene(SDL_Renderer *renderer) {
    //below will load the textures
    startButtonTexture = IMG_LoadTexture(renderer, "../assets/textures/StartScene/startButton.png");
    quitButtonTexture = IMG_LoadTexture(renderer, "../assets/textures/StartScene/quitButton.png");

    //pos
    startButtonRect = {150,60,500,400};
    quitButtonRect={150,260,500,400};
}
StartScene::~StartScene(){
    SDL_DestroyTexture(startButtonTexture);
    SDL_DestroyTexture(quitButtonTexture);
}
void StartScene::enter() {
    std::cout << "entered start scene\n";
}
void StartScene::handleEvents(SDL_Event &e) {
    if(e.type == SDL_MOUSEBUTTONDOWN){
        int x = e.button.x;
        int y = e.button.y;

        //this shall be the start buttton click
        if (x>= startButtonRect.x && x<= startButtonRect.x + startButtonRect.w
        && y>= startButtonRect.y && y <= startButtonRect.y + startButtonRect.h) {
            std::cout << "start button was clicked!";
        }
        if (x>= quitButtonRect.x && x<= quitButtonRect.x + quitButtonRect.w
            && y>= quitButtonRect.y && y <= quitButtonRect.y + quitButtonRect.h) {
            std::cout << "quit button was clicked!";
            SDL_Quit();
            std::exit(0);
        }
    }
}
void StartScene::update(float dt) {

}


void StartScene::render(SDL_Renderer *renderer) {
    SDL_RenderCopy(renderer,startButtonTexture, nullptr, &startButtonRect);
    SDL_RenderCopy(renderer, quitButtonTexture, nullptr,&quitButtonRect);
}
void StartScene::exit() {
    std::cout << "exit start scene";
}