#include "SpareRoom.h"
#include "sceneManager.h"
#include <iostream>

SpareRoom::SpareRoom(SDL_Renderer *renderer, StoryFlags &flags, DialogueSystem *dialogue) :
renderer(renderer), storyFlags(flags), dialogueSystem(dialogue){}


void SpareRoom::enter() {
    std::cout << "SpareRoom::enter()\n";
    inspector->loadItems("../assets/data/SpareRoom.json",renderer);
    if (storyFlags.getFlag("ReachedMaxwellDoor") &&
        !storyFlags.getFlag("EnteredMaxwellRoom"))
    {
        storyFlags.setFlag("EnteredMaxwellRoom", true);
        sceneManager->changeScene(SceneID::SCENE_MAXWELLROOM, renderer);
    }

}
void SpareRoom::handleEvents(SDL_Event &e) {}
void SpareRoom::update(float dt) {}
void SpareRoom::render(SDL_Renderer* renderer, bool debugMode) {}
void SpareRoom::exit() {
std::cout << "SpareRoom::exit()\n";
}