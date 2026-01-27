//
// Created by Lou halpin on 22/01/2026.
//

#ifndef AMIPRETTY_EDITORSCENE_H
#define AMIPRETTY_EDITORSCENE_H

#include <SDL.h>
#include <vector>
#include "scene.h"
#include "sceneManager.h"
#include "storyFlags.h"
#include "dialogueSystem.h"
#include "inspectionSystem.h"

class editorScene : public Scene {
public:
    editorScene(SDL_Renderer* renderer, StoryFlags& flags, DialogueSystem* dialogue);
    ~editorScene();


    void enter() override;
    void handleEvents(SDL_Event& e) override;
    //just in case
    void update(float dt) override;
    void render(SDL_Renderer* renderer) override;
    void exit() override;

    //attempting fix
    void setSceneManager(SceneManager* mgr) override { sceneManager = mgr;}


private:

    SDL_Renderer* renderer;
    SceneManager* sceneManager = nullptr;

    StoryFlags& storyFlags;
    DialogueSystem* dialogueSystem;

    std::vector<Item> items; //for json
    Item* selectedItem = nullptr;

    std::string currentTexture = "testPlayer.png"; //default png
};

#endif //AMIPRETTY_EDITORSCENE_H
