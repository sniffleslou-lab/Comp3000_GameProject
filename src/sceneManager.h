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

    void changeScene(SceneID nextScene);

    void update(float dt);
    void render(SDL_Renderer* renderer);

private:
    SceneID currentScene;

};
