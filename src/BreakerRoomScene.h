//
// Created by Lou halpin on 22/03/2026.
//

#ifndef AMIPRETTY_BREAKERROOMSCENE_H
#define AMIPRETTY_BREAKERROOMSCENE_H
#pragma once
#include <SDL.h>
#include "scene.h"
#include "character.h"
#include "controls.h"
#include "storyFlags.h"
#include "sceneManager.h"
#include "inspectionSystem.h"
#include "dialogueSystem.h"
class BreakerRoomScene : public Scene {
    public:
    BreakerRoomScene(SDL_Renderer* renderer, StoryFlags& flags, DialogueSystem* dialogue);
    ~BreakerRoomScene();

    void enter() override;
    void handleEvents(SDL_Event& e) override;
    void update(float dt) override;
    void render(SDL_Renderer* renderer, bool debugMode) override;
    void exit() override;

    void setSceneManager(SceneManager* mgr) { sceneManager = mgr; }

private:
    SDL_Renderer* renderer;
    StoryFlags& storyFlags;
    DialogueSystem* dialogueSystem;
    SceneManager* sceneManager = nullptr;

    std::unique_ptr<Character> player;
    std::unique_ptr<inspectionSystem> inspector;
    SDL_Texture* roomTexture;

    Controls controls;
};

#endif //AMIPRETTY_BREAKERROOMSCENE_H