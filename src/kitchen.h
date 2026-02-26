//
// Created by Lou halpin on 03/12/2025.
//

#ifndef AMIPRETTY_KITCHEN_H
#define AMIPRETTY_KITCHEN_H
#include <SDL.h>
#include "scene.h"
#include <memory>
#include "controls.h"
#include "character.h"
#include "inspectionSystem.h"
#include "dialogueSystem.h"
#include "storyFlags.h"
#include "NPC.h"


class Kitchen : public Scene{
public:
    Kitchen(SDL_Renderer* renderer, StoryFlags& flags, DialogueSystem* dialogue, SceneManager* sm);
    ~Kitchen();

    void enter() override;
    void handleEvents(SDL_Event& e) override;
    void update(float dt) override;
    void render(SDL_Renderer* renderer) override;
    void exit() override;

private:
    SDL_Renderer* renderer;
    StoryFlags& storyFlags;
    SceneManager* sceneManager = nullptr;
    std::unique_ptr<Character> player;
    Controls controls;
    std::unique_ptr<inspectionSystem> inspector;
    DialogueSystem* dialogueSystem;
    std::unique_ptr<NPC> garretNPC;
    bool eWasDown = false;
    bool startDialogueNextFrame = false;
};

#endif //AMIPRETTY_KITCHEN_H
