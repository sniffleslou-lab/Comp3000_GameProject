//
// Created by Lou halpin on 03/12/2025.
//

#include "hallwayA.h"
#include <iostream>

HallwayA::HallwayA(SDL_Renderer *renderer, StoryFlags &flags)
:storyFlags(flags), renderer(renderer)
{
    player = std::make_unique<Character>(renderer,"../assets/textures/testPlayer.png",100,200);
    inspector= std::make_unique<inspectionSystem>(renderer);
    inspector->loadItems("../assets/data/item.json",renderer);

    dialogueSystem = std::make_unique<DialogueSystem>(storyFlags);
    dialogueSystem->loadAllDialogue("../assets/data/dialogue/");
}

HallwayA::~HallwayA() {}

void HallwayA::enter() {
    std::cout<<"entered hallway scene\n";
}

void HallwayA::handleEvents(SDL_Event &e) {
    controls.handleInput(e, *player, *inspector);

    if (e.type == SDL_KEYDOWN && e.key.keysym.sym==SDLK_e){
       // inspector->inspect(player->getPosition(),*sceneManager, renderer);
    }

    if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_f){
        dialogueSystem->startDialogue("Garret");
    }
    if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE){
        dialogueSystem->nextLine();
    }
}

void HallwayA::update(float dt) {}
void HallwayA::render(SDL_Renderer *renderer) {}
void HallwayA::exit() {}