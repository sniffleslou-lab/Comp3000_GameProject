//
// Created by Lou halpin on 17/11/2025.
//
#include <nlohmann/json.hpp>
#include <fstream>
#include "inspectionSystem.h"
#include <iostream>
#include "sceneManager.h"
#include <SDL_image.h>
#include "storyFlags.h"

inspectionSystem::inspectionSystem(SDL_Renderer *renderer, StoryFlags& flags, DialogueSystem* dialogue) : storyFlags(flags), dialogueSystem(dialogue){
    font = TTF_OpenFont("../assets/font/Sunlight Dreams.otf", 20);
    if (!font){
        std::cerr<<"failed to load inpsection font: " << TTF_GetError()<< std::endl;
    }

}
inspectionSystem::~inspectionSystem() {
    if (font) TTF_CloseFont(font);

}

///loads thejson file
void inspectionSystem::loadItems(const std::string &jsonPath, SDL_Renderer *renderer) {

    items.clear();

    std::ifstream file(jsonPath);

    if (!file.is_open()){
        std::cerr << "failed to open item file : " << jsonPath << std::endl;
    }
    nlohmann::json data;
    file >> data;
    for(auto& listitem : data["items"]){
        Item item;
        item.id = std::stoi(listitem["id"].get<std::string>());
        item.name = listitem["name"];
        item.inspect = listitem["inspect"];

        item.type = listitem.value("type", "item");
        item.targetScene = listitem.value("targetScene", "");
        item.flag = listitem.value("flag", "");

        item.rect = {listitem["x"],listitem["y"],listitem["w"],listitem["h"]};


        std::string texturePath = "../assets/textures/" + listitem["texture"].get<std::string>();
        item.texture = IMG_LoadTexture(renderer,texturePath.c_str());


        //Hiding items when player picsk them up
       if(!item.flag.empty() && storyFlags.getFlag(item.flag)){
           item.rect = {0,0,0,0};
       }

        items.push_back(item);
    }
}
void inspectionSystem::update(float dt) {
    if (inspectActive){
        inspectTimer += dt;
        if (inspectTimer >= 5.0f){
            currentText.clear();
            inspectActive=false;
        }
    }
    if (doorCooldown){
        doorCooldownTimer += dt;
        if (doorCooldownTimer >= 0.5f){
            doorCooldown = false;
        }
    }
}

void inspectionSystem::render(SDL_Renderer *renderer) {
    for (auto &item: items) {
        if (item.texture) {
            SDL_RenderCopy(renderer, item.texture, nullptr, &item.rect);
        }
    else {
        std::cerr << "Warning: failed to load texture for item: " << item.name << std::endl;

    }
}
    if (!currentText.empty()&&font){
        SDL_Rect box = {50,300,700,100};
        SDL_SetRenderDrawColor(renderer, 0,0,0,200);
        SDL_RenderFillRect(renderer, &box);

        SDL_Color white = {255,255,255,255};
        SDL_Surface* surface = TTF_RenderText_Blended(font, currentText.c_str(),white);
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer,surface);

        SDL_Rect dst={70,320, surface->w,surface->h};
        SDL_RenderCopy(renderer,texture, nullptr, &dst);

        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
    }
}

bool inspectionSystem::isNear(const std::string &itemName, const SDL_Rect &playerRect) {
    for(auto& item : items){
        if(item.name == itemName){
            SDL_Rect itemRect = item.rect;
            return SDL_HasIntersection(&playerRect, &itemRect);
        }
    }
    return false;
}
void inspectionSystem::inspect(const SDL_Rect &playerPos, SceneManager &sceneManager, SDL_Renderer *renderer) {
    if(doorCooldown){
        return;
    }

    for (auto& item:items) {
        if(item.rect.w == 0 || item.rect.h ==0)continue;

        if(SDL_HasIntersection(&playerPos,&item.rect)){
            //for npc door
            if (item.type == "npcdoor"){
                if(storyFlags.getFlag("AnnaQuestAccepted")){
                    std::cout << "knocking on Maxwell's door \n";
                    dialogueSystem->startDialogue("Maxwell");
                }else
                {
                    currentText = "it's locked.";
                    inspectActive = true;
                    inspectTimer = 0.0f;
                }
                return;
            }
            if(item.type== "door" && !doorCooldown){
                SceneID target = sceneManager.sceneIdFromString(item.targetScene);
                std::cout<< "door to " << item.targetScene << " triggered\n";
                sceneManager.changeScene(target,renderer);

                doorCooldown = true;
                doorCooldownTimer = 0.0f;
                return;
            }else {
                //test
                currentText = item.inspect;
                std::cout << "player says: " << item.inspect << std::endl;
                inspectTimer=0.0f;
                inspectActive =true;

                //now questing
                if(!item.flag.empty()) {
                    storyFlags.setFlag(item.flag, true);
                    item.rect = {0, 0, 0, 0};
                    if (item.flag == "PickedUp_batteries") {
                        storyFlags.setFlag("AnnaUnlocked", true);
                    }
                }

            }
        }
    }
}
