#include "NPC.h"
#include <SDL_image.h>
#include <iostream>

NPC::NPC(SDL_Renderer *renderer, const std::string &texturePath, int x, int y) {
    SDL_Surface* surface = IMG_Load(texturePath.c_str());
    if(!surface){
        std::cerr << "failed to load npc texture: " << IMG_GetError()<< std::endl;
        return;
    }

    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    rect.x=x;
    rect.y=y;
    rect.w=64;
    rect.h=64;
}
NPC::~NPC() {
    if(texture)SDL_DestroyTexture(texture);
}
void NPC::draw(SDL_Renderer *renderer) {
    if(texture){
        SDL_RenderCopy(renderer, texture, nullptr, &rect);
    }
}