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

class SpareRoom : public Scene {
    public:
    SpareRoom(SDL_Renderer* renderer, StoryFlags& flags, DialogueSystem* dialogue);
    ~SpareRoom() {}

    void enter() override;
    void update(float dt) override;
    void render(SDL_Renderer *renderer) override;
    void exit() override;
    void handleEvents(SDL_Event &e) override;

    void setSceneManager(SceneManager* mgr) override { sceneManager = mgr; }

    private:
    StoryFlags& storyFlags;
    DialogueSystem* dialogueSystem;
    SDL_Renderer* renderer;
    std::unique_ptr<inspectionSystem> inspector;
    SceneManager* sceneManager = nullptr;

};



#endif //AMIPRETTY_SPAREROOM_H