//
// Created by Lou halpin on 24/03/2026.
//

#ifndef AMIPRETTY_SPAREROOM_H
#define AMIPRETTY_SPAREROOM_H

#include "scene.h"
#include "storyFlags.h"
#include "dialogueSystem.h"
#include <SDL.h>
#include "inspectionSystem.h"
#include "character.h"
#include "sceneManager.h"
#include "controls.h"

class SpareRoom : public Scene {
    public:
    SpareRoom(SDL_Renderer* renderer, StoryFlags& flags, DialogueSystem* dialogue);
    ~SpareRoom() {}

    void enter() override;
    void update(float dt) override;
    void render(SDL_Renderer *renderer, bool debugMode) override;
    void exit() override;
    void handleEvents(SDL_Event &e) override;

    void setSceneManager(SceneManager* mgr) override { sceneManager = mgr; }

    private:
    SDL_Renderer* renderer;
    StoryFlags& storyFlags;
    DialogueSystem* dialogueSystem;
    SceneManager* sceneManager = nullptr;

    std::unique_ptr<Character> player;
    std::unique_ptr<inspectionSystem> inspector;

    Controls controls;
    SDL_Texture* darknessOverlay = nullptr;


};



#endif //AMIPRETTY_SPAREROOM_H