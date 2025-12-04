//
// Created by Lou halpin on 17/11/2025.
//for scene control
//
#include "sceneManager.h"
#include <iostream>
#include "StartScene.h"
#include "bedroom.h"
#include "hallwayA.h"
#include "kitchen.h"

SceneManager::SceneManager(SDL_Renderer *renderer, StoryFlags& flags)  : storyFlags(flags) {
    currentSceneID = SceneID::SCENE_START;
    currentScene = createScene(currentSceneID,renderer);
    if (currentScene)currentScene->enter();
}
void SceneManager::changeScene(SceneID newScene, SDL_Renderer *renderer)  {
    unloadScene();
    currentSceneID = newScene;
    currentScene = createScene(newScene, renderer);
}
std::unique_ptr<Scene>SceneManager::createScene(SceneID scene, SDL_Renderer *renderer)  {
    switch (scene) {
        case SceneID::SCENE_START:
            std::cout << "loaded the start scene\n";
            return std::make_unique<StartScene>(renderer, *this);
        case SceneID::SCENE_BEDROOM:
            std::cout << "loaded the bedroom scene\n";
            return std::make_unique<bedroom>(renderer, storyFlags);
        case SceneID::SCENE_HALLWAYA:
            std::cout<<"loaded the halway scene \n";
            return  std::make_unique<HallwayA>(renderer, storyFlags);
        case SceneID::SCENE_KITCHEN:
            std::cout<<"loaded the kitchen scene\n";
            return std::make_unique<Kitchen>(renderer, storyFlags);
        default:
            return nullptr;
    }
}
void SceneManager::unloadScene() {
    //delete scene
    currentScene.reset();
}


void SceneManager::update(float dt) {
    if (currentScene) currentScene->update(dt);

}
void SceneManager::handleEvents(SDL_Event &e) {
    if (currentScene) currentScene->handleEvents(e);

}

void SceneManager::render(SDL_Renderer *renderer) {
    if (currentScene) currentScene->render(renderer);

}

SceneID SceneManager::sceneIdFromString(const std::string &name) {
    if (name== "SCENE_START")return SceneID::SCENE_START;
    if (name== "SCENE_BEDROOM")return SceneID::SCENE_BEDROOM;
    if (name== "SCENE_HALLWAYA")return SceneID::SCENE_HALLWAYA;
    if (name== "SCENE_KITCHEN")return SceneID::SCENE_KITCHEN;
    return SceneID::SCENE_START;

}