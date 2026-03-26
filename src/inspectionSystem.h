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
#include <SDL_image.h>
//same as json
struct Item{
    int id;
    std::string name;
    std::string inspect;
    SDL_Rect rect;
    SDL_Texture* texture;
    std::string type;
    std::string targetScene;
    std::string flag;
    int layer = 0;

};

class inspectionSystem{
public:
    inspectionSystem(SDL_Renderer* renderer, StoryFlags& flags, DialogueSystem* dialogueSystem);
    ~inspectionSystem();

    void loadItems(const std::string& jsonPath, SDL_Renderer* renderer);
    void render(SDL_Renderer* renderer);
    void update(float dt, const SDL_Rect& playerPos);
    void inspect(const SDL_Rect& playerPos, SceneManager& sceneManager, SDL_Renderer* renderer);

    const std::vector<Item>& getItems() const { return items;}

    //cooldown for transition
    bool doorCooldown=false;
    float doorCooldownTimer=0.0f;
    bool isNear(const std::string& itemName, const SDL_Rect& playerRect);

    void updatePrompt(const SDL_Rect& playerPos);

private:
    std::vector<Item> items;
    std::string currentText;
    TTF_Font* font = nullptr;

    //timer for the inspector
    float inspectTimer = -0.0f;
    float inspectActive = false;


    StoryFlags& storyFlags;
    DialogueSystem* dialogueSystem;

    //prompt
    bool showPrompt = false;
    std::string promptText;
    SDL_Rect lastPlayerPos;

    SDL_Texture* keyIcon = nullptr;

    float bounceTimer = 0.0f;
};
#endif //AMIPRETTY_INSPECTIONSYSTEM_H
