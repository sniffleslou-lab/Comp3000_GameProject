//
// Created by Lou halpin on 21/03/2026.
//

#ifndef AMIPRETTY_ARC2HALLWAYSCENE_H
#define AMIPRETTY_ARC2HALLWAYSCENE_H


#pragma once
#include <SDL.h>
#include "scene.h"
#include "character.h"
#include "storyFlags.h"
#include "sceneManager.h"
#include "inspectionSystem.h"
#include "dialogueSystem.h"
#include "controls.h"
class Arc2HallwayScene : public Scene {
    public:
    Arc2HallwayScene(SDL_Renderer* renderer, StoryFlags& storyFlags, DialogueSystem* dialogue);
    ~Arc2HallwayScene();

    void enter() override;
    void exit() override;
    void update(float dt) override;
    void render(SDL_Renderer *renderer, bool debugMode) override;
    void handleEvents(SDL_Event &e) override;

    void setSceneManager(SceneManager* mgr) { sceneManager = mgr; }

    private:
    SDL_Renderer* renderer;
    StoryFlags& storyFlags;
    DialogueSystem* dialogueSystem;

    std::unique_ptr<Character> player;
    std::unique_ptr<inspectionSystem> inspector;

    Controls controls;

    SDL_Texture* wallTexture;

    SDL_Rect breakerDoor;

    bool startDialogueNextFrame = false;
    bool anxietyTriggered = false;
};
#endif //AMIPRETTY_ARC2HALLWAYSCENE_H