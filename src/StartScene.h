//
// Created by Lou halpin on 18/11/2025.
//

#ifndef AMIPRETTY_STARTSCENE_H
#define AMIPRETTY_STARTSCENE_H

#include <SDL.h>
#include "scene.h"
#include "sceneManager.h"

class StartScene: public Scene{
public:
    StartScene(SDL_Renderer* renderer, SceneManager& manager);
    ~StartScene();

    void enter() override;
    void handleEvents(SDL_Event& e) override;
   //just in case
   void update(float dt) override;
    void render(SDL_Renderer* renderer) override;
    void exit() override;

private:
    SDL_Texture* startButtonTexture;
    SDL_Rect startButtonRect;

    SDL_Texture* quitButtonTexture;
    SDL_Rect quitButtonRect;

    SceneManager& sceneManager;
    SDL_Renderer* renderer;



};

#endif //AMIPRETTY_STARTSCENE_H
