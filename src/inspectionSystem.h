//
// Created by Lou halpin on 19/11/2025.
//

#ifndef AMIPRETTY_INSPECTIONSYSTEM_H
#define AMIPRETTY_INSPECTIONSYSTEM_H
#include <SDL.h>
#include <string>
#include <vector>

//same as json
struct Item{
    int id;
    std::string name;
    std::string inspect;
    SDL_Rect rect;
    SDL_Texture* texture;
};

class inspectionSystem{
public:
    inspectionSystem(SDL_Renderer* renderer);
    ~inspectionSystem();

    void loadItems(const std::string& jsonPath, SDL_Renderer* renderer);
    void render(SDL_Renderer* renderer);
    void inspect(const SDL_Rect& playerPos);

private:
    std::vector<Item> items;
};
#endif //AMIPRETTY_INSPECTIONSYSTEM_H
