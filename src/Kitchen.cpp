//
// Created by Lou halpin on 03/12/2025.
//
#include "kitchen.h"
#include <iostream>

Kitchen::Kitchen(SDL_Renderer *renderer, StoryFlags &flags):storyFlags(flags) {
    player = std::make_unique<Character>(renderer,"../assets/textures/testPlayer.png",100,200);
    inspector= std::make_unique<inspectionSystem>(renderer);
    inspector->loadItems("../assets/data/item.json",renderer);

    dialogueSystem = std::make_unique<DialogueSystem>(storyFlags);
    dialogueSystem->loadAllDialogue("../assets/data/dialogue/");
}

Kitchen::~Kitchen() {}
void Kitchen::enter() {}
void Kitchen::handleEvents(SDL_Event &e) {}
void Kitchen::update(float dt) {}
void Kitchen::render(SDL_Renderer *renderer) {}
void Kitchen::exit() {}