//
// Created by Lou halpin on 17/11/2025.
//
#include <nlohmann/json.hpp>
#include <fstream>
#include "inspectionSystem.h"
#include <iostream>
#include <SDL_image.h>

inspectionSystem::inspectionSystem(SDL_Renderer *renderer) {
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
void inspectionSystem::inspect(const SDL_Rect &playerPos) {
    for (auto& item:items) {
        if(SDL_HasIntersection(&playerPos,&item.rect)){
            //test
            currentText = item.inspect;
            std::cout << "player says: "<< item.inspect << std::endl;
        }
    }
}
