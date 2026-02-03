#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include "inspectionSystem.h"

class Character{
public:
    Character(SDL_Renderer* renderer, const std::string& imagePath, int x,int y);
    ~Character();

    void draw();
    void move(int dx, int dy, const std::vector<Item>& items);
    SDL_Rect getPosition()const;


private:
    SDL_Renderer* renderer;
    SDL_Texture* texture;
    SDL_Rect position;

};