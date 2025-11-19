//
// Created by Lou halpin on 17/11/2025.
//for scene control
//
#include "sceneManager.h"
#include <iostream>
#include "StartScene.h"
#include "bedroom.h"

SceneManager::SceneManager(SDL_Renderer *renderer)  {
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

            return std::make_unique<bedroom>(renderer);
            break;
        default:
            break;
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