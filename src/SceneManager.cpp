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
#include "editorScene.h"
#include "BreakerRoomScene.h"
#include "Arc2HallwayScene.h"
#include "MaxwellRoom.h"
#include "SpareRoom.h"

SceneManager::SceneManager(SDL_Renderer *renderer, StoryFlags& flags, DialogueSystem* dialogue)
: storyFlags(flags), dialogueSystem(dialogue) {

    currentSceneID = SceneID::SCENE_START;
    currentScene = createScene(currentSceneID,renderer);
    if (currentScene)currentScene->enter();
}
void SceneManager::changeScene(SceneID newScene, SDL_Renderer *renderer)  {
    dialogueSystem->endDialogue();
    unloadScene();
    currentSceneID = newScene;
    currentScene = createScene(newScene, renderer);
    if (currentScene) currentScene->enter();
}
std::unique_ptr<Scene>SceneManager::createScene(SceneID scene, SDL_Renderer *renderer)  {
    switch (scene) {
        case SceneID::SCENE_START: {
            std::cout << "loaded the start scene\n";
            auto s = std::make_unique<StartScene>(renderer,storyFlags,dialogueSystem);
            s->setSceneManager(this);
            return s;
        }
        case SceneID::SCENE_BEDROOM: {
            std::cout << "loaded the bedroom scene\n";
            auto s = std::make_unique<bedroom>(renderer, storyFlags,dialogueSystem);
            s->setSceneManager(this);
            return s;
        }
        case SceneID::SCENE_HALLWAYA: {
            std::cout << "loaded the halway scene \n";
            auto s= std::make_unique<HallwayA>(renderer, storyFlags,dialogueSystem);
            s->setSceneManager(this);
            return s;
        }
        case SceneID::SCENE_KITCHEN: {
            std::cout << "loaded the kitchen scene\n";
            auto s= std::make_unique<Kitchen>(renderer, storyFlags,dialogueSystem);
            s->setSceneManager(this);
            return s;
        }
        case SceneID::SCENE_ARC2_HALLWAY: {
            std::cout << "loaded Arc2 hallway scene\n";
            auto s = std::make_unique<Arc2HallwayScene>(renderer, storyFlags, dialogueSystem);
            s->setSceneManager(this);
            return s;
        }
        case SceneID::SCENE_BREAKERROOM: {
            std::cout << "loaded breaker room scene\n";
            auto s = std::make_unique<BreakerRoomScene>(renderer, storyFlags, dialogueSystem);
            s->setSceneManager(this);
            return s;
        }
        case SceneID::SCENE_MAXWELLROOM: {
            std::cout << "loaded MAX room scene\n";
            auto s = std::make_unique<MaxwellRoom>(renderer, storyFlags, dialogueSystem);
            s->setSceneManager(this);
            return s;
        }
        case SceneID::SCENE_EDITOR: {
            std::cout << "loaded spare room scene\n";
            auto s = std::make_unique<SpareRoom>(renderer, storyFlags, dialogueSystem);
            s->setSceneManager(this);
            return s;
        }

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
    if (name== "SCENE_EDITOR") return SceneID::SCENE_EDITOR;
    if (name== "SCENE_ARC2_HALLWAY")return SceneID::SCENE_ARC2_HALLWAY;
    if (name == "SCENE_BREAKERROOM") return SceneID::SCENE_BREAKERROOM;
    if (name == "SCENE_MAXWELLROOM")return SceneID::SCENE_MAXWELLROOM;
    if (name == "SCENE_SPAREROOM")return SceneID::SCENE_SPAREROOM;

    return SceneID::SCENE_START;

}