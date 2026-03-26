//
// Created by Lou halpin on 18/11/2025.
//

#ifndef AMIPRETTY_STARTSCENE_H
#define AMIPRETTY_STARTSCENE_H

#include <SDL.h>
#include "scene.h"
#include "storyFlags.h"
#include "dialogueSystem.h"

class StartScene: public Scene{
public:
    StartScene(SDL_Renderer* renderer, StoryFlags& flags, DialogueSystem* dialogue);
    ~StartScene();

    void enter() override;
    void handleEvents(SDL_Event& e) override;
   //just in case
   void update(float dt) override;
    void render(SDL_Renderer* renderer, bool debugMode) override;
    void exit() override;

private:

    SDL_Renderer* renderer;
    SDL_Texture* startButtonTexture;
    SDL_Rect startButtonRect;

    SDL_Texture* quitButtonTexture;
    SDL_Rect quitButtonRect;

    SDL_Texture* logoTexture;
    SDL_Rect LogoRect;

    StoryFlags& storyFlags;
    DialogueSystem* dialogueSystem;



};

#endif //AMIPRETTY_STARTSCENE_H
