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

    garretNPC = std::make_unique<NPC>(
        renderer,"../assets/textures/Characters/garretCha.png",300,250);

    darknessOverlay = IMG_LoadTexture(renderer, "../assets/textures/dark_overlay.png");
    if (!darknessOverlay) {
        std::cerr << "Failed to load darkness overlay\n";
    }

}

BreakerRoomScene::~BreakerRoomScene() {
    SDL_DestroyTexture(roomTexture);
}

void BreakerRoomScene::enter() {
    std::cout<<"Entering the breaker room\n";
    inspector->doorCooldown = true;
    inspector->doorCooldownTimer = 0.0f;
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
    inspector->update(dt, player->getPosition());

    //start breaker dialogue
    if (!storyFlags.getFlag("BreakerDialogueDone") &&
        !dialogueSystem->isActive) {
        dialogueSystem->startDialogue("BreakerBoxScene");
        storyFlags.setFlag("BreakerDialogueDone", true);
    }
    //leave the room after fixing breaker
    if (storyFlags.getFlag("BreakerFixed")&&
        !dialogueSystem->isActive) {
        sceneManager->changeScene(SceneID::SCENE_ARC2_HALLWAY, renderer);
    }

}
    void BreakerRoomScene::render(SDL_Renderer* renderer, bool debugMode) {
        SDL_RenderCopy(renderer, roomTexture, NULL, NULL);

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0,0,0,140);
    SDL_Rect darken = {0,0,800,600};
    SDL_RenderFillRect(renderer, &darken);
    //breaker
    SDL_SetRenderDrawColor(renderer, 255, 200, 50, 80); // warm glow

    SDL_Rect glow = { 540, 240, 140, 220 };
    SDL_RenderFillRect(renderer, &glow);

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 220);
    SDL_Rect fullDark = {0, 0, 800, 600};
    SDL_RenderFillRect(renderer, &fullDark);

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);

    SDL_Rect p = player->getPosition();
    SDL_Rect spotlight = {
        p.x + p.w/2 - 150,
        p.y + p.h/2 - 150,
        300,
        300
    };

    SDL_RenderFillRect(renderer, &spotlight);

    inspector->render(renderer);
    garretNPC->draw(renderer);
    player->draw();
    dialogueSystem->render(renderer);

    //debug
    if (debugMode) {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

        for (auto& item : inspector->getItems()) {
            SDL_RenderDrawRect(renderer, &item.rect);
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);

        SDL_Rect p = player->getPosition();
        SDL_RenderDrawRect(renderer, &p);
    }
    }




