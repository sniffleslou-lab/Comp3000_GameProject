//
// Created by Lou halpin on 17/11/2025.
//for scene control
//
#include "sceneManager.h"
#include "iostream"

SceneManager::SceneManager() {
    currentScene = SceneID::SCENE_START;
}
void SceneManager::changeScene(SceneID newScene) {
    UnloadScene(currentScene);
    currentScene = newScene;
    loadScene(newScene);
}
void SceneManager::loadScene(SceneID scene) {
    switch (scene) {
        case SceneID::SCENE_START:
            std::cout << "loaded the start scene\n";
            break;
        default:
            break;
    }
}


void SceneManager::update(float dt) {

}

void SceneManager::render(SDL_Renderer *renderer) {

}