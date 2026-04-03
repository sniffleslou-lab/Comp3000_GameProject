//
// Created by Lou halpin on 21/03/2026.
//

#include "Arc2HallwayScene.h"

#include <iostream>
#include <SDL_image.h>


Arc2HallwayScene::Arc2HallwayScene(SDL_Renderer* renderer, StoryFlags& flags, DialogueSystem* dialogue)
: renderer(renderer), storyFlags(flags), dialogueSystem(dialogue)
{
    player = std::make_unique<Character>(
        renderer,
        "../assets/textures/Characters/playerChar.png",
        100, 450
    );

    wallTexture = IMG_LoadTexture(renderer, "../assets/textures/wall.png");

    inspector = std::make_unique<inspectionSystem>(renderer, storyFlags, dialogueSystem);
    inspector->loadItems("../assets/data/arc2hallway.json", renderer);

    startDialogueNextFrame = false;
    anxietyTriggered = false;
}


Arc2HallwayScene::~Arc2HallwayScene() {
    SDL_DestroyTexture(wallTexture);
}


void Arc2HallwayScene::enter() {
    std::cout << "Entered Arc2 hallway\n";
    inspector->doorCooldown = true;
    inspector->doorCooldownTimer = 0.0f;
    if (sceneManager == nullptr)
        std::cout << "ERROR: sceneManager is NULL in Arc2HallwayScene!\n";

}

void Arc2HallwayScene::handleEvents(SDL_Event& e) {

    if (dialogueSystem->choiceActive) {
        if (e.type == SDL_KEYDOWN && e.key.repeat == 0) {

            if (e.key.keysym.sym == SDLK_a)
                dialogueSystem->selectedChoice = 0;

            if (e.key.keysym.sym == SDLK_d)
                dialogueSystem->selectedChoice = 1;

            if (e.key.keysym.sym == SDLK_SPACE) {
                Choice chosen = dialogueSystem->currentChoices[dialogueSystem->selectedChoice];
                storyFlags.setFlag(chosen.flag, true);
                dialogueSystem->choiceActive = false;
                dialogueSystem->justFinishedChoice = true;

                if (!chosen.next.empty())
                    dialogueSystem->jumpToLine(chosen.next);
                else
                    dialogueSystem->nextLine();
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

        if (!dialogueSystem->choiceActive)
            dialogueSystem->nextLine();
    }
}

void Arc2HallwayScene::update(float dt) {
    inspector->update(dt, player->getPosition());

    //axienty scene after fixing breaker
    if (storyFlags.getFlag("BreakerFixed") &&
        !storyFlags.getFlag("Arc2_AnxietySeen") &&
        !dialogueSystem->isActive) {
        dialogueSystem->startDialogue("HalPlayThoughts");
        storyFlags.setFlag("Arc2_AnxietySeen", true);
    }

}

void Arc2HallwayScene::render(SDL_Renderer* renderer, bool debugMode) {
    SDL_SetRenderDrawColor(renderer, 20, 20, 25, 255);
    SDL_RenderClear(renderer);


    inspector->render(renderer);
    player->draw();
    dialogueSystem->render(renderer);

    if (storyFlags.getFlag("BreakerFixed")) {
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 120);
        SDL_Rect darkOverlay = {0, 0, 800, 600};
        SDL_RenderFillRect(renderer, &darkOverlay);
    }

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

void Arc2HallwayScene::exit() {
    std::cout << "Left Arc2 hallway\n";
}

