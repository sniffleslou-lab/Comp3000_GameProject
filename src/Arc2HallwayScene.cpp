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
        100, 300
    );

    wallTexture = IMG_LoadTexture(renderer, "../assets/textures/wall.png");

    inspector = std::make_unique<inspectionSystem>(renderer, storyFlags, dialogueSystem);
    inspector->loadItems("../assets/data/arc2hallway.json", renderer);

    breakerDoor = { 1100, 200, 80, 200 };
}


Arc2HallwayScene::~Arc2HallwayScene() {
    SDL_DestroyTexture(wallTexture);
}


void Arc2HallwayScene::enter() {
    std::cout << "Entered Arc2 hallway\n";
    inspector->doorCooldown = true;
    inspector->doorCooldownTimer = 0.0f;
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

    //Breaker dialogue trigger
    if (startDialogueNextFrame) {
        dialogueSystem->startDialogue("BreakerBoxScene");
        startDialogueNextFrame = false;
    }

    SDL_Rect p = player->getPosition();

    // Trigger breaker dialogue when near door
    if (SDL_HasIntersection(&p, &breakerDoor) && !dialogueSystem->isActive) {
        startDialogueNextFrame = true;
    }


}

void Arc2HallwayScene::render(SDL_Renderer* renderer, bool debugMode) {
    SDL_SetRenderDrawColor(renderer, 20, 20, 25, 255);
    SDL_RenderClear(renderer);

    SDL_Rect topWall = { 0, 0, 1280, 100 };
    SDL_Rect bottomWall = { 0, 620, 1280, 100 };

    SDL_RenderCopy(renderer, wallTexture, NULL, &topWall);
    SDL_RenderCopy(renderer, wallTexture, NULL, &bottomWall);
    inspector->render(renderer);
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

void Arc2HallwayScene::exit() {
    std::cout << "Left Arc2 hallway\n";
}

