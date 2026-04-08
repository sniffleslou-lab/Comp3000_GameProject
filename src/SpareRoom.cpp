#include "SpareRoom.h"
#include "sceneManager.h"
#include <iostream>

SpareRoom::SpareRoom(SDL_Renderer *renderer, StoryFlags &flags, DialogueSystem *dialogue) :
renderer(renderer), storyFlags(flags), dialogueSystem(dialogue) {
    player = std::make_unique<Character>(renderer,
        "../assets/textures/Characters/playerChar.png", 200, 300);

    inspector = std::make_unique<inspectionSystem>(renderer, storyFlags, dialogueSystem);

    darknessOverlay = IMG_LoadTexture(renderer, "../assets/textures/dark_overlay.png");

}


void SpareRoom::enter() {
    std::cout << "SpareRoom::enter()\n";
    inspector->loadItems("../assets/data/SpareRoom.json",renderer);
   /* if (storyFlags.getFlag("ReachedMaxwellDoor") &&
        !storyFlags.getFlag("EnteredMaxwellRoom"))
    {
        storyFlags.setFlag("EnteredMaxwellRoom", true);
        sceneManager->changeScene(SceneID::SCENE_MAXWELLROOM, renderer);
    }*/

}
void SpareRoom::handleEvents(SDL_Event &e) {
    // Player movement
    controls.handleInput(e, *player, *inspector);

    // Interact
    if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_e) {
        inspector->inspect(player->getPosition(), *sceneManager, renderer);
    }
}
void SpareRoom::update(float dt) {
    inspector->update(dt, player->getPosition());
}
void SpareRoom::render(SDL_Renderer* renderer, bool debugMode) {

    // Draw room normally
    inspector->render(renderer);
    player->draw();

    // Draw darkness overlay
    SDL_SetTextureBlendMode(darknessOverlay, SDL_BLENDMODE_BLEND);
    SDL_SetTextureAlphaMod(darknessOverlay, 180);
    SDL_RenderCopy(renderer, darknessOverlay, NULL, NULL);

    // Light circle around player
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);

    SDL_Rect light = {
        player->getPosition().x - 120,
        player->getPosition().y - 120,
        240,
        240
    };

    SDL_RenderFillRect(renderer, &light);

    dialogueSystem->render(renderer);
}
void SpareRoom::exit() {
std::cout << "SpareRoom::exit()\n";
}