//
// Created by Lou halpin on 03/12/2025.
//

#include "hallwayA.h"
#include <iostream>

HallwayA::HallwayA(SDL_Renderer *renderer, StoryFlags &flags, DialogueSystem* dialogue)
:storyFlags(flags), renderer(renderer), dialogueSystem(dialogue)
{
    player = std::make_unique<Character>(renderer,"../assets/textures/testPlayer.png",100,200);
    inspector= std::make_unique<inspectionSystem>(renderer, storyFlags);
    inspector->loadItems("../assets/data/hallwayA.json",renderer);

    //dialogueSystem = std::make_unique<DialogueSystem>(storyFlags);
    //dialogueSystem->loadAllDialogue("../assets/data/dialogue/");
}

HallwayA::~HallwayA() {}

void HallwayA::enter() {
    std::cout<<"entered hallway scene\n";
    inspector->doorCooldown = true;
    inspector->doorCooldownTimer = 0.0f;
}

void HallwayA::handleEvents(SDL_Event &e) {

    if(dialogueSystem->choiceActive){
        if (e.type == SDL_KEYDOWN){
            if (e.key.keysym.sym == SDLK_a){
                dialogueSystem->selectedChoice=0;
            }
            if (e.key.keysym.sym == SDLK_d){
                dialogueSystem->selectedChoice=1;
            }
            if (e.key.keysym.sym == SDLK_SPACE){
                //NOW confirming
                Choice chosen =dialogueSystem->currentChoices[dialogueSystem->selectedChoice];
                storyFlags.setFlag(chosen.flag, true);
                dialogueSystem->choiceActive=false;
                dialogueSystem->currentChoices.clear();
                dialogueSystem->nextLine();
            }
        }
        return;
    }

    controls.handleInput(e, *player, *inspector);

    if (e.type == SDL_KEYDOWN && e.key.keysym.sym==SDLK_e){
        inspector->inspect(player->getPosition(),*sceneManager, renderer);
    }

    if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_f){
        queuedNPC = "Garret";
        startDialogueNextFrame = true;
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

void HallwayA::update(float dt) {
    inspector->update(dt);
    if (startDialogueNextFrame){
        dialogueSystem->startDialogue(queuedNPC);
        startDialogueNextFrame = false;
    }

}
void HallwayA::render(SDL_Renderer *renderer) {
    inspector->render(renderer);
    player->draw();
    dialogueSystem->render(renderer);
}
void HallwayA::exit() {
    std::cout<<"left hallway scene\n";
}