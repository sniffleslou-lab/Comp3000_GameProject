//
// Created by Lou halpin on 17/11/2025.
//
#include <nlohmann/json.hpp>
#include <fstream>
#include "inspectionSystem.h"
#include <iostream>
#include <SDL_image.h>

inspectionSystem::inspectionSystem(SDL_Renderer *renderer) {}
inspectionSystem::~inspectionSystem() {

}

///loads thejson file
void inspectionSystem::loadItems(const std::string &jsonPath, SDL_Renderer *renderer) {
    std::ifstream file("../assets/data/item.json");
    nlohmann::json data;
    file >> data;

    for(auto& listitem : data["items"]){
        Item item;
        item.id = std::stoi(listitem["id"].get<std::string>());
        item.name = listitem["name"];
        item.inspect = listitem["inspect"];
        item.rect = {listitem["x"],listitem["y"],listitem["w"],listitem["h"]};

        std::string texturePath = "../assets/textures/" + listitem["texture"].get<std::string>();
        item.texture = IMG_LoadTexture(renderer,texturePath.c_str());


        items.push_back(item);
    }
}
void inspectionSystem::render(SDL_Renderer *renderer) {
    for (auto& item:items) {
        SDL_RenderCopy(renderer,item.texture, nullptr, &item.rect);
    }
}
void inspectionSystem::inspect(const SDL_Rect &playerPos) {
    for (auto& item:items) {
        if(SDL_HasIntersection(&playerPos,&item.rect)){
            //test
            std::cout << "player says: "<< item.inspect << std::endl;
        }
    }
}
