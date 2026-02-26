//
// Created by Lou halpin on 03/12/2025.
//

#include "hallwayA.h"
#include <iostream>

HallwayA::HallwayA(SDL_Renderer *renderer, StoryFlags &flags, DialogueSystem* dialogue)
:storyFlags(flags), renderer(renderer), dialogueSystem(dialogue)
{
    player = std::make_unique<Character>(renderer,"../assets/textures/testPlayer.png",100,200);
    inspector= std::make_unique<inspectionSystem>(renderer, storyFlags, dialogueSystem);
    inspector->loadItems("../assets/data/hallwayA.json",renderer);

    if(storyFlags.getFlag("AnnaUnlocked")&& !storyFlags.getFlag("AnnaMoved")){
        annaNPC = std::make_unique<NPC>(renderer, "../assets/textures/wall.png", 500, 300);
    }
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
            if (e.key.repeat != 0) return;
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
                dialogueSystem->justFinishedChoice= true;

                if(!chosen.next.empty()){
                    dialogueSystem->jumpToLine(chosen.next);
                } else{
                    dialogueSystem->nextLine();
                }
            }
        }
        return;
    }

    controls.handleInput(e, *player, *inspector);

    if (e.type == SDL_KEYDOWN && e.key.keysym.sym==SDLK_e){
        inspector->inspect(player->getPosition(),*sceneManager, renderer);
    }

    if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_f){
        if (annaNPC && playerIsNearAnna()){
            queuedNPC = "Anna";
            startDialogueNextFrame = true;
            return;
        }
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

bool HallwayA::playerIsNearAnna() {
    SDL_Rect p = player->getPosition();
    SDL_Rect a = annaNPC->getRect();
    return SDL_HasIntersection(&p,&a);
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
    if (annaNPC) annaNPC->draw(renderer);
}
void HallwayA::exit() {
    std::cout<<"left hallway scene\n";
}