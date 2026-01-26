//
// Created by Lou halpin on 19/11/2025.
//

#ifndef AMIPRETTY_BEDROOM_H
#define AMIPRETTY_BEDROOM_H
#include <SDL.h>
#include "scene.h"
#include <memory>
#include "character.h"//going to load character into bedroom for testing
#include "controls.h"
#include "inspectionSystem.h"
#include "dialogueSystem.h"

class bedroom: public Scene{
public:
    bedroom(SDL_Renderer* renderer, StoryFlags& flags, DialogueSystem* dialogue);
    ~bedroom();

    void enter() override;
    void handleEvents(SDL_Event& e) override;
    //just in case
    void update(float dt) override;
    void render(SDL_Renderer* renderer) override;
    void exit() override;

    //attempting fix
    void setSceneManager(SceneManager* mgr) override { sceneManager = mgr;}

private:
    StoryFlags& storyFlags;
    std::unique_ptr<Character> player;
    Controls controls;
    std::unique_ptr<inspectionSystem> inspector;
    DialogueSystem* dialogueSystem;

    SDL_Renderer* renderer;
    SceneManager* sceneManager= nullptr;

};
#endif //AMIPRETTY_BEDROOM_H
