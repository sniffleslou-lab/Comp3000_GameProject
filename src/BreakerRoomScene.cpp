//
// Created by Lou halpin on 22/03/2026.
//

#include "BreakerRoomScene.h"
#include <SDL_image.h>
#include <iostream>

BreakerRoomScene::BreakerRoomScene(SDL_Renderer *renderer, StoryFlags &flags, DialogueSystem *dialogue) : renderer(renderer), storyFlags(flags), dialogueSystem(dialogue)

{
    player = std::make_unique<Character>(renderer, "../assets/textures/Characters/playerChar.png", 200, 300);

    roomTexture = IMG_LoadTexture(renderer, "../assets/textures/breaker_room.png");

    inspector = std::make_unique<inspectionSystem>(renderer, storyFlags, dialogueSystem);
    inspector->loadItems("../assets/data/breakerRoom.json", renderer);
}

BreakerRoomScene::~BreakerRoomScene() {
    SDL_DestroyTexture(roomTexture);
}

void BreakerRoomScene::enter() {
    std::cout<<"Entering the breaker room\n";
}

void BreakerRoomScene::exit() {}

void BreakerRoomScene::handleEvents(SDL_Event &e) {
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
                Choice chosen = dialogueSystem->currentChoices[dialogueSystem->selectedChoice];

                storyFlags.setFlag(chosen.flag, true);

                dialogueSystem->choiceActive = false;
                dialogueSystem->justFinishedChoice = true;

                if (!chosen.next.empty()) {
                    dialogueSystem->jumpToLine(chosen.next);
                } else {
                    dialogueSystem->nextLine();
                }
            }
        }
        return;
    }
    controls.handleInput(e, *player, *inspector);

    if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_e) {
        inspector->inspect(player->getPosition(), *sceneManager, renderer);
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

void BreakerRoomScene::update(float dt) {
    inspector->update(dt);
    if (storyFlags.getFlag("BreakerFixed")) {
        sceneManager->changeScene(SceneID::SCENE_ARC2_HALLWAY, renderer);
    }
}
    void BreakerRoomScene::render(SDL_Renderer* renderer) {
        SDL_RenderCopy(renderer, roomTexture, NULL, NULL);

        inspector->render(renderer);
        player->draw();
        dialogueSystem->render(renderer);
    }




