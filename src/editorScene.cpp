//
// Created by Lou halpin on 27/01/2026.
//


#include "editorScene.h"
#include <iostream>
#include <SDL_image.h>
#include <nlohmann/json.hpp>
#include <fstream>


editorScene::editorScene(SDL_Renderer *renderer, StoryFlags &flags, DialogueSystem *dialogue) :
renderer(renderer), storyFlags(flags), dialogueSystem(dialogue){}

editorScene::~editorScene() {}

void editorScene::enter() {
    std::cout << "entered editor scene\n";
}
void editorScene::handleEvents(SDL_Event &e) {
    if (e.type == SDL_MOUSEBUTTONDOWN){
        int mx = e.button.x;
        int my = e.button.y;

        //blow is for placing items
        if (e.button.button == SDL_BUTTON_LEFT) {
            Item item;
            item.id = items.size() + 1;
            item.name = "new item";
            item.inspect = "inspect text";
            item.rect = {mx,my,64,64};
            item.texture = IMG_LoadTexture(renderer, ("../assets/textures/" + currentTexture).c_str());
            item.type = "item";
            item.targetScene = "";

            items.push_back(item);
        }
    }
    //now below we will make it that it will save with f5
    if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_F5)
    {
        nlohmann::json data;
        data["items"] = nlohmann::json::array();
        for (auto& item : items){
            data["items"].push_back({
                {"id", std::to_string(item.id)},
                {"name", item.name},
                {"inspect", item.inspect},
                {"x", item.rect.x},
                {"y", item.rect.y},
                {"w", item.rect.w},
                {"h", item.rect.h},
                {"texture", currentTexture},
                {"type", item.type},
                {"targetScene", item.targetScene}
            });
        }
        std::ofstream out("../assets/data/edited.json");
        out << data.dump(4);
        std::cout << "saved test.json\n";
    }
    //return to start with esc
    if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE){

        sceneManager->changeScene(SceneID::SCENE_START, renderer);
    }
}
void editorScene::update(float dt) {}
void editorScene::render(SDL_Renderer *renderer, bool debugMode) {
    for (auto& item : items){
        if (item.texture){
            SDL_RenderCopy(renderer, item.texture, nullptr, &item.rect);
        }
    }
}
void editorScene::exit() {
    std::cout<< "exited editor scene\n";
}