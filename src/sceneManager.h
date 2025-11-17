#ifndef AMIPRETTY_SCENEMANAGER_H
#define AMIPRETTY_SCENEMANAGER_H

#include <SDL.h>


enum SceneID{
    SCENE_START,
    SCENE_BEDROOM
};

class SceneManager{
public:
    SceneManager();
    //this will be used to switch scenes
    void changeScene(SceneID newScene);


    void update(float dt);
    void render(SDL_Renderer* renderer);

private:
    SceneID currentScene;

     void loadScene(SceneID scene);
     void UnloadScene(SceneID scene);

};
