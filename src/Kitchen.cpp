//
// Created by Lou halpin on 03/12/2025.
//
#include "kitchen.h"
#include <iostream>

Kitchen::Kitchen(SDL_Renderer *renderer, StoryFlags &flags, DialogueSystem* dialogue, SceneManager* sm)
:storyFlags(flags), renderer(renderer), dialogueSystem(dialogue), sceneManager(sm) {
    player = std::make_unique<Character>(renderer,"../assets/textures/testPlayer.png",100,200);
    inspector= std::make_unique<inspectionSystem>(renderer,storyFlags);
    inspector->loadItems("../assets/data/kitchen.json",renderer);

    garretNPC = std::make_unique<NPC>(renderer, "../assets/textures/wall.png",300,200);


    //dialogueSystem = std::make_unique<DialogueSystem>(storyFlags);
    //dialogueSystem->loadAllDialogue("../assets/data/dialogue/");
}

Kitchen::~Kitchen() {}
void Kitchen::enter() {
    std::cout<< "entered kitchen scene";
   // dialogueSystem->startDialogue("Garret");
    inspector->doorCooldown = true;
    inspector->doorCooldownTimer = 0.0f;

    startDialogueNextFrame = true;
}
void Kitchen::handleEvents(SDL_Event &e) {


    if (dialogueSystem->choiceActive) {
        if (e.type == SDL_KEYDOWN) {
            if (e.key.repeat != 0) return;

            if (e.key.keysym.sym == SDLK_a) {
                dialogueSystem->selectedChoice = 0;
            }

            if (e.key.keysym.sym == SDLK_d) {
                dialogueSystem->selectedChoice = 1;
            }
            if (e.key.keysym.sym == SDLK_SPACE) {
                std::cout << "SPACE pressed while choiceActive = "
                          << dialogueSystem->choiceActive << "\n";
                Choice chosen = dialogueSystem->currentChoices[dialogueSystem->selectedChoice];
                storyFlags.setFlag(chosen.flag, true);

                dialogueSystem -> choiceActive = false;
                dialogueSystem -> justFinishedChoice = true;
                //dialogueSystem -> currentChoices.clear();
                dialogueSystem -> nextLine();
            }
        }
        return;
    }

    controls.handleInput(e, *player, *inspector);

    if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_e) {
        inspector->inspect(player->getPosition(), *sceneManager, renderer);

    }
    if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_f) {
        dialogueSystem->startDialogue("Garret");
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


void Kitchen::update(float dt) {
inspector->update(dt);
    //garrets dialogue start when the player walks into the kitchen
    if (startDialogueNextFrame){
       dialogueSystem->startDialogue("Garret");
        startDialogueNextFrame = false;
    }
/*
    if(!dialogueSystem->choiceActive) {
        const Uint8* keys = SDL_GetKeyboardState(NULL);
        bool eDown = keys[SDL_SCANCODE_E];
        if (eDown && !eWasDown) {
            dialogueSystem->nextLine();
        }
        eWasDown = eDown;
    }*/

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