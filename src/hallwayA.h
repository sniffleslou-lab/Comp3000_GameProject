//
// Created by Lou halpin on 03/12/2025.
//

#ifndef AMIPRETTY_HALLWAYA_H
#define AMIPRETTY_HALLWAYA_H
#include <SDL.h>
#include "scene.h"
#include <memory>
#include "controls.h"
#include "character.h"
#include "inspectionSystem.h"
#include "dialogueSystem.h"
#include "storyFlags.h"
#include "sceneManager.h"


class HallwayA : public Scene{
public:
    HallwayA(SDL_Renderer* renderer, StoryFlags& flags);
    ~HallwayA();

    void enter() override;
    void handleEvents(SDL_Event& e) override;
    void update(float dt) override;
    void render(SDL_Renderer* renderer) override;
    void exit() override;
    void setSceneManager(SceneManager* mgr) { sceneManager = mgr; }

private:
    StoryFlags& storyFlags;

    SDL_Renderer* renderer;
    SceneManager* sceneManager= nullptr;

    std::unique_ptr<Character> player;
    Controls controls;
    std::unique_ptr<inspectionSystem> inspector;
    std::unique_ptr<DialogueSystem> dialogueSystem;

};

#endif //AMIPRETTY_HALLWAYA_H
