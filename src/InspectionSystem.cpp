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

    keyIcon = IMG_LoadTexture(renderer,"../assets/textures/key_E.png");

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
        item.layer = listitem.value("layer",0);

        item.rect = {listitem["x"],listitem["y"],listitem["w"],listitem["h"]};


        std::string texturePath = "../assets/textures/" + listitem["texture"].get<std::string>();
        item.texture = IMG_LoadTexture(renderer,texturePath.c_str());


        //Hiding items when player picsk them up
       if(!item.flag.empty() && storyFlags.getFlag(item.flag)){
           item.rect = {0,0,0,0};
       }

        items.push_back(item);
    }
    //laying for object
    std::sort(items.begin(),items.end(),[](const Item& a, const Item& b){
        return a.layer < b.layer;
    });
}
void inspectionSystem::update(float dt, const SDL_Rect& playerPos) {
    lastPlayerPos = playerPos;
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
    updatePrompt(lastPlayerPos);
    bounceTimer += dt * 2.0f;

}
void inspectionSystem::updatePrompt(const SDL_Rect &playerPos) {
    showPrompt = false;
    SDL_Rect detectBox = playerPos;
    detectBox.x -= 10;
    detectBox.y -= 10;
    detectBox.w += 20;
    detectBox.h += 20;
    for (auto& item : items) {
        if (item.rect.w == 0 || item.rect.h == 0)continue;

        if (SDL_HasIntersection(&detectBox, &item.rect)) {
            if (item.type == "item"|| item.type == "solid_item") {
                showPrompt = true;
                promptText = "Press E to inspect";
                return;
            }
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
    if (showPrompt && keyIcon) {
        SDL_Rect iconRect;
        iconRect.w = 32;
        iconRect.h = 32;


        int baseX = lastPlayerPos.x + (lastPlayerPos.w/2) -(iconRect.w/2);
        int baseY = lastPlayerPos.y - 40;
        float bounceOffset =  sin(bounceTimer) * 8.0f;

        iconRect.x = baseX;
        iconRect.y = baseY + bounceOffset;
        SDL_RenderCopy(renderer, keyIcon, nullptr, &iconRect);
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
    if (doorCooldown) {
        return;
    }
        SDL_Rect detectBox = playerPos;
        detectBox.x -= 10;
        detectBox.y -= 10;
        detectBox.w += 20;
        detectBox.h += 20;

    for (auto& item : items) {
        if (item.rect.w == 0 || item.rect.h == 0) continue;

        if (SDL_HasIntersection(&detectBox, &item.rect)) {

            // NPC door logic
            if (item.type == "npcdoor") {
                if (storyFlags.getFlag("AnnaQuestAccepted")) {
                    std::cout << "knocking on Maxwell's door \n";
                    dialogueSystem->startDialogue("Maxwell");
                } else {
                    currentText = "it's locked.";
                    inspectActive = true;
                    inspectTimer = 0.0f;
                }
                return;
            }
            if (item.type == "door" && !doorCooldown) {
                SceneID target = sceneManager.sceneIdFromString(item.targetScene);
                std::cout << "door to " << item.targetScene << " triggered\n";
                sceneManager.changeScene(target, renderer);

                doorCooldown = true;
                doorCooldownTimer = 0.0f;
                return;
            }

            if (item.type == "item" || item.type == "solid_item") {

                currentText = item.inspect;
                std::cout << "player says: " << item.inspect << std::endl;
                inspectTimer = 0.0f;
                inspectActive = true;

                if (!item.flag.empty()) {
                    storyFlags.setFlag(item.flag, true);
                    item.rect = {0, 0, 0, 0};

                    if (item.flag == "PickedUp_batteries") {
                        storyFlags.setFlag("AnnaUnlocked", true);
                    }
                }
                return;
            }
        }
    }
}
