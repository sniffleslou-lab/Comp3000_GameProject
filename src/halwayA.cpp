//
// Created by Lou halpin on 03/12/2025.
//

#include "hallwayA.h"
#include <iostream>

HallwayA::HallwayA(SDL_Renderer *renderer, StoryFlags &flags, DialogueSystem* dialogue)
:storyFlags(flags), renderer(renderer), dialogueSystem(dialogue)
{
    player = std::make_unique<Character>(renderer,"../assets/textures/Characters/playerChar.png",100,200);
    inspector= std::make_unique<inspectionSystem>(renderer, storyFlags, dialogueSystem);
    inspector->loadItems("../assets/data/hallwayA.json",renderer);


    bool unlocked = storyFlags.getFlag("AnnaUnlocked");
    bool moved = storyFlags.getFlag("AnnaMoved");
    std::cout << "DEBUG: annaUnlocked= " << unlocked << " | AnnaMoved=" << moved << "\n";

    if (unlocked && !moved){
        annaNPC = std::make_unique<NPC>(renderer, "../assets/textures/Characters/annaCha.png", 500,300);
        std::cout << "anna spawned in hallway\n";
    }
    //dialogueSystem = std::make_unique<DialogueSystem>(storyFlags);
    //dialogueSystem->loadAllDialogue("../assets/data/dialogue/");
}

HallwayA::~HallwayA() {}

void HallwayA::enter() {
    std::cout<<"entered hallway scene\n";


    //rests diaglogue state
    queuedNPC = "";
    startDialogueNextFrame = false;
    //dialogueSystem->isActive = false;
    //dialogueSystem->choiceActive = false;
    //dialogueSystem->justFinishedChoice = false;

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
        if(inspector->isNear("MaxwellDoor", player->getPosition())){
            queuedNPC = "Maxwell";
            startDialogueNextFrame = true;
            return;
        }

        /*if(playerIsNearGarret()){
            queuedNPC = "Garret";
            startDialogueNextFrame = true;
            return;
        }*/
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
    //maxwell hallway moment in arc2
    if (storyFlags.getFlag("Arc2ArgumentSeen")&&
        !storyFlags.getFlag("Arc2_MaxwellSeen")&&
        inspector->isNear("MaxwellTrigger", player->getPosition())&&
        !dialogueSystem->isActive) {
        dialogueSystem->startDialogue("MaxwellHall");
        storyFlags.setFlag("Arc2_MaxwellSeen", true);
    }

}
void HallwayA::render(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 80,60,100,255);
    SDL_RenderClear(renderer);

    inspector->render(renderer);
    player->draw();
    if (annaNPC) annaNPC->draw(renderer);
    dialogueSystem->render(renderer);


}
void HallwayA::exit() {
    std::cout<<"left hallway scene\n";
}