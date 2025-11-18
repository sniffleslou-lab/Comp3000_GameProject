//
// Created by Lou halpin on 18/11/2025.
// creating a base scene class to call for
//

#ifndef AMIPRETTY_SCENE_H
#define AMIPRETTY_SCENE_H
#include <SDL.h>

class Scene {
public:
    virtual ~Scene(){}
    virtual void enter() = 0;
    virtual void handleEvents(SDL_Event& e) = 0;
    virtual void update(float dt) =0;
    virtual void render(SDL_Renderer* renderer)=0;
    virtual void exit()=0;

};


#endif //AMIPRETTY_SCENE_H
