#include "character.h"
#include <iostream>

Character::Character(SDL_Renderer *renderer, const std::string &imagePath, int x, int y)
:renderer(renderer)
{
    SDL_Surface* surface = IMG_Load(imagePath.c_str());
    if (!surface){
        std::cerr<< "failure to add image to character. : " << IMG_GetError()<< std::endl;
        texture = nullptr;
        position={x,y,64,64};
        return;
    }
    texture = SDL_CreateTextureFromSurface(renderer,surface);


    position.x=x;
    position.y=y;
    position.w = 64;
    position.h = 64;
    SDL_FreeSurface(surface);
}
Character::~Character() {
    if (texture)SDL_DestroyTexture(texture);
}
void Character::move(int dx, int dy){
    position.x += dx;
    position.y +=dy;
}
void Character::draw() {
    if(texture) SDL_RenderCopy(renderer,texture, nullptr, &position);
}

SDL_Rect Character::getPosition() const{
    return position;
}