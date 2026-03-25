//
// Created by Lou halpin on 24/03/2026.
//

#ifndef AMIPRETTY_MAXWELLROOM_H
#define AMIPRETTY_MAXWELLROOM_H

#include "scene.h"
#include "storyFlags.h"
#include "dialogueSystem.h"
#include <SDL.h>
#include "inspectionSystem.h"

class MaxwellRoom : public Scene {
public:
    MaxwellRoom(SDL_Renderer* renderer, StoryFlags& flags, DialogueSystem* dialogue);
    ~MaxwellRoom();
    void enter() override;
    void update(float dt) override;
    void handleEvents(SDL_Event &e) override;
    void render(SDL_Renderer* renderer) override;
    void exit() override;

    void setSceneManager(SceneManager* mgr) { sceneManager = mgr; }


private:
    StoryFlags& storyFlags;
    DialogueSystem* dialogueSystem;
    std::unique_ptr<inspectionSystem> inspector;
    SDL_Renderer* renderer;
    SceneManager* sceneManager = nullptr;
};

#endif //AMIPRETTY_MAXWELLROOM_H