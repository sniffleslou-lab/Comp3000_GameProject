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
#include "QTEManager.h"
#include  "NPC.h"
#include "Character.h"
#include "controls.h"

class MaxwellRoom : public Scene {
public:
    MaxwellRoom(SDL_Renderer* renderer, StoryFlags& flags, DialogueSystem* dialogue);
    ~MaxwellRoom();
    void enter() override;
    void update(float dt) override;
    void handleEvents(SDL_Event &e) override;
    void render(SDL_Renderer* renderer, bool debugMode) override;
    void exit() override;

    void setSceneManager(SceneManager* mgr) { sceneManager = mgr; }


private:
    SDL_Renderer* renderer;
    StoryFlags& storyFlags;
    DialogueSystem* dialogueSystem = nullptr;

    std::unique_ptr<inspectionSystem> inspector;
    std::unique_ptr<QTEManager> qteManager;

    SceneManager* sceneManager = nullptr;
    std::unique_ptr<Character> player;
    std::unique_ptr<NPC> maxwellNPC;
    Controls controls;

    bool startQTE_NextFrame = false;
    bool replayQTE_NextFrame = false;
};

#endif //AMIPRETTY_MAXWELLROOM_H