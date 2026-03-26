//
// Created by Lou halpin on 18/11/2025.
//
#include "StartScene.h"
#include "sceneManager.h"
#include <SDL_image.h>
#include <iostream>


StartScene::StartScene(SDL_Renderer* renderer, StoryFlags& flags, DialogueSystem* dialogue)
: renderer(renderer), storyFlags(flags), dialogueSystem(dialogue)
{

    //below will load the textures
    startButtonTexture = IMG_LoadTexture(renderer, "../assets/textures/StartScene/startButton.png");
    quitButtonTexture = IMG_LoadTexture(renderer, "../assets/textures/StartScene/quitButton.png");
    logoTexture = IMG_LoadTexture(renderer, "../assets/textures/StartScene/logo.png");
    //pos
    startButtonRect = {250,280,300,80};
    quitButtonRect={250,380,300,80};
    LogoRect = {150,80,500,150};
}
StartScene::~StartScene(){
    SDL_DestroyTexture(startButtonTexture);
    SDL_DestroyTexture(quitButtonTexture);
    SDL_DestroyTexture(logoTexture);
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
            sceneManager->changeScene(SceneID::SCENE_BEDROOM,renderer);
        }
        if (x>= quitButtonRect.x && x<= quitButtonRect.x + quitButtonRect.w
            && y>= quitButtonRect.y && y <= quitButtonRect.y + quitButtonRect.h) {
            std::cout << "quit button was clicked!";
            SDL_Quit();
            std::exit(0);
        }
    }
    if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_F1){

        sceneManager->changeScene(SceneID::SCENE_EDITOR, renderer);
    }
}
void StartScene::update(float dt) {

}


void StartScene::render(SDL_Renderer *renderer, bool debugMode) {
    //background colour
    SDL_SetRenderDrawColor(renderer, 80,60,100,255);
    SDL_RenderClear(renderer);

    SDL_RenderCopy(renderer,startButtonTexture, nullptr, &startButtonRect);
    SDL_RenderCopy(renderer, quitButtonTexture, nullptr,&quitButtonRect);
    SDL_RenderCopy(renderer, logoTexture, nullptr,&LogoRect);

}
void StartScene::exit() {
    std::cout << "exit start scene";
}