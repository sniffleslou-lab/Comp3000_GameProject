//
// Created by Lou halpin on 03/12/2025.
//
#include "kitchen.h"
#include <iostream>

Kitchen::Kitchen(SDL_Renderer *renderer, StoryFlags &flags, DialogueSystem* dialogue)
:storyFlags(flags), renderer(renderer), dialogueSystem(dialogue) {
    player = std::make_unique<Character>(renderer,"../assets/textures/testPlayer.png",100,200);
    inspector= std::make_unique<inspectionSystem>(renderer);
    inspector->loadItems("../assets/data/kitchen.json",renderer);

    garretNPC = std::make_unique<NPC>(renderer, "../assets/textures/wall.png",300,200);

    //dialogueSystem = std::make_unique<DialogueSystem>(storyFlags);
    //dialogueSystem->loadAllDialogue("../assets/data/dialogue/");
}

Kitchen::~Kitchen() {}
void Kitchen::enter() {
    std::cout<< "entered kitchen scene";
}
void Kitchen::handleEvents(SDL_Event &e) {}
void Kitchen::update(float dt) {
    static bool startedGarretDialogue = false;

    //garrets dialogue start when the player walks into the kitchen
    if (!startedGarretDialogue){
        dialogueSystem->startDialogue("Garret");
        startedGarretDialogue;
    }
    const Uint8* keys = SDL_GetKeyboardState(NULL);
    if (keys[SDL_SCANCODE_E]){
        dialogueSystem->nextLine();
    }
}
void Kitchen::render(SDL_Renderer *renderer) {
    inspector->render(renderer);
    garretNPC->draw(renderer);
    player->draw();
    dialogueSystem->render(renderer);
}
void Kitchen::exit() {
    std::cout<< "left kitchen scene";

}