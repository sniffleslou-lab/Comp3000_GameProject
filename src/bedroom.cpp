//
// Created by Lou halpin on 19/11/2025.
//
#include "bedroom.h"
#include <iostream>

bedroom::bedroom(SDL_Renderer *renderer) {
    player = std::make_unique<Character>(renderer,"../assets/textures/testPlayer.png",100,200);
    inspector= std::make_unique<inspectionSystem>(renderer);
    inspector->loadItems("../assets/data/item.json",renderer);
}
bedroom::~bedroom()  {}

void bedroom::enter() {
    std::cout<< "entered bedroom scene";
}
void bedroom::handleEvents(SDL_Event &e) {
    controls.handleInput(e, *player, *inspector);

    if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_f){
        dialogueSystem->startDialogue("Garret");
    }
    if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE){
        dialogueSystem->nextLine();
    }
}
void bedroom::update(float dt) {}

void bedroom::render(SDL_Renderer *renderer) {
    inspector->render(renderer);
    player->draw();

    dialogueSystem->render(renderer);
}
void bedroom::exit() {
    std::cout<<"exited bedroom scene";
}