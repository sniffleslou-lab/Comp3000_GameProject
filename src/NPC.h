#pragma once
#include <SDL.h>
#include <string>

class NPC{
public:
    NPC(SDL_Renderer* renderer, const std::string& texturePath, int x, int y);
    ~NPC();

    void draw(SDL_Renderer* renderer);
    SDL_Rect getRect() const { return rect; }


private:
    SDL_Texture* texture = nullptr;
    SDL_Rect rect;
};