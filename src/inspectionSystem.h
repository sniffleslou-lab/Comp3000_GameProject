//
// Created by Lou halpin on 19/11/2025.
//

#ifndef AMIPRETTY_INSPECTIONSYSTEM_H
#define AMIPRETTY_INSPECTIONSYSTEM_H
#include <SDL.h>
#include <string>
#include <vector>
#include <SDL_ttf.h>
#include "sceneManager.h"
#include "storyFlags.h"
//same as json
struct Item{
    int id;
    std::string name;
    std::string inspect;
    SDL_Rect rect;
    SDL_Texture* texture;
    std::string type;
    std::string targetScene;

};

class inspectionSystem{
public:
    inspectionSystem(SDL_Renderer* renderer, StoryFlags& flags);
    ~inspectionSystem();

    void loadItems(const std::string& jsonPath, SDL_Renderer* renderer);
    void render(SDL_Renderer* renderer);
    void update(float dt);
    void inspect(const SDL_Rect& playerPos, SceneManager& sceneManager, SDL_Renderer* renderer);

    const std::vector<Item>& getItems() const { return items;}

private:
    std::vector<Item> items;
    std::string currentText;
    TTF_Font* font = nullptr;

    //timer for the inspector
    float inspectTimer = -0.0f;
    float inspectActive = false;
    //cooldown for transition
    bool doorCooldown=false;
    float doorCooldownTimer=0.0f;

    StoryFlags& storyFlags;
};
#endif //AMIPRETTY_INSPECTIONSYSTEM_H
