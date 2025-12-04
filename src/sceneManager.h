#ifndef AMIPRETTY_SCENEMANAGER_H
#define AMIPRETTY_SCENEMANAGER_H

#include <SDL.h>
#include "scene.h"
#include <memory>
#include "storyFlags.h"


enum SceneID{
    SCENE_START,
    SCENE_BEDROOM,
    SCENE_KITCHEN,
    SCENE_HALLWAYA
};

class SceneManager{
public:
    SceneManager(SDL_Renderer* renderer, StoryFlags& flags);
    //this will be used to switch scenes
    void changeScene(SceneID newScene, SDL_Renderer* renderer);
    void update(float dt);
    void handleEvents(SDL_Event& e);
    void render(SDL_Renderer* renderer);
    SceneID sceneIdFromString(const std::string& name);


private:
    SceneID currentSceneID;

     //void loadScene(SceneID scene);
     std::unique_ptr<Scene> currentScene;
     std::unique_ptr<Scene> createScene(SceneID scene,SDL_Renderer* renderer);
     void unloadScene();
     StoryFlags& storyFlags;

};
#endif