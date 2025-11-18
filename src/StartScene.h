//
// Created by Lou halpin on 18/11/2025.
//

#ifndef AMIPRETTY_STARTSCENE_H
#define AMIPRETTY_STARTSCENE_H

#include <SDL.h>
#include "sceneManager.h"
#include "scene.h"

class StartScene : public Scene{
public:
    void enter() override;
    void handleEvents(SDL_Event& e) override;
    void update(float dt) override;
    void render(SDL_Renderer* renderer) override;
    void exit() override;



};

#endif //AMIPRETTY_STARTSCENE_H
