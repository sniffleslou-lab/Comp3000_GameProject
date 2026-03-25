#include "MaxwellRoom.h"
#include "sceneManager.h"
#include <iostream>

MaxwellRoom::MaxwellRoom(SDL_Renderer* renderer, StoryFlags& flags, DialogueSystem* dialogue)
: renderer(renderer), storyFlags(flags), dialogueSystem(dialogue) {}

MaxwellRoom::~MaxwellRoom() {

}

void MaxwellRoom::enter() {
    std::cout << "Entering Maxwells room\n";

    //load room
    inspector->loadItems("../assets/data/MaxwellRoom.json", renderer);
    //trigger for the confrontation
    if (!storyFlags.getFlag("MaxwellStormedOut")) {
        dialogueSystem->startDialogue("MaxwellConfrontationScene");
    }
}
void MaxwellRoom::handleEvents(SDL_Event &e) {
    if(dialogueSystem->choiceActive){
        if (e.type == SDL_KEYDOWN){
            if (e.key.repeat != 0) return;

            if (e.key.keysym.sym == SDLK_a){
                dialogueSystem->selectedChoice = 0;
            }
            if (e.key.keysym.sym == SDLK_d){
                dialogueSystem->selectedChoice = 1;
            }
            if (e.key.keysym.sym == SDLK_SPACE){
                Choice chosen = dialogueSystem->currentChoices[dialogueSystem->selectedChoice];
                storyFlags.setFlag(chosen.flag, true);
                dialogueSystem->choiceActive = false;
                dialogueSystem->justFinishedChoice = true;

                if(!chosen.next.empty()){
                    dialogueSystem->jumpToLine(chosen.next);
                } else {
                    dialogueSystem->nextLine();
                }
            }
        }
        return;
    }
    if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE) {

        if (dialogueSystem->justFinishedChoice) {
            dialogueSystem->justFinishedChoice = false;
            return;
        }

        if (!dialogueSystem->choiceActive) {
            dialogueSystem->nextLine();
        }
    }
}
void MaxwellRoom::update(float dt) {
    //after maxwell storms out, will auto transition back to the hallway
    if (storyFlags.getFlag("MaxwellStormedOut")&&
        !dialogueSystem->isActive) {
        sceneManager->changeScene(SceneID::SCENE_HALLWAYA, renderer);
    }
}
void MaxwellRoom::render(SDL_Renderer* renderer) {}

void MaxwellRoom::exit() {
    std::cout << "Exiting Maxwells room\n";
}
