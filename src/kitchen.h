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


class Kitchen : public Scene{
public:
    Kitchen(SDL_Renderer* renderer, StoryFlags& flags);
    ~Kitchen();

    void enter() override;
    void handleEvents(SDL_Event& e) override;
    void update(float dt) override;
    void render(SDL_Renderer* renderer) override;
    void exit() override;

private:
    StoryFlags& storyFlags;
    std::unique_ptr<Character> player;
    Controls controls;
    std::unique_ptr<inspectionSystem> inspector;
    std::unique_ptr<DialogueSystem> dialogueSystem;

};

#endif //AMIPRETTY_KITCHEN_H
