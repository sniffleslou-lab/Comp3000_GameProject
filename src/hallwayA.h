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
#include "NPC.h"


class HallwayA : public Scene{
public:
    HallwayA(SDL_Renderer* renderer, StoryFlags& flags, DialogueSystem* dialogue);
    ~HallwayA();

    void enter() override;
    void handleEvents(SDL_Event& e) override;
    void update(float dt) override;
    void render(SDL_Renderer* renderer) override;
    void exit() override;
    void setSceneManager(SceneManager* mgr) { sceneManager = mgr; }
    bool playerIsNearAnna();
    bool playerIsNearGarret();


private:
    StoryFlags& storyFlags;
    DialogueSystem* dialogueSystem;


    std::unique_ptr<Character> player;
    std::unique_ptr<inspectionSystem> inspector;
    std::unique_ptr<NPC> annaNPC;

    SDL_Renderer* renderer;
    SceneManager* sceneManager= nullptr;

    Controls controls;
    bool startDialogueNextFrame = false;
    std::string queuedNPC;

};

#endif //AMIPRETTY_HALLWAYA_H
