//
// Created by Lou halpin on 28/11/2025.
//

#ifndef AMIPRETTY_DIALOGUESYSTEM_H
#define AMIPRETTY_DIALOGUESYSTEM_H

#include "storyFlags.h"
#include <string>
#include <vector>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
struct Choice{
        std::string text;
        std::string flag;
        std::string next;
};
struct dialogueLine{
    std::string id;
    std::string text;
    std::string condition;
    std::string speaker;
    std::string next;
    std::string flag;
    std::vector<Choice>choices;
};
struct NPCDialogue{
    std::string npcId;
    std::string displayName;
    std::string requiresFlag;
    std::vector<dialogueLine> lines;
};


class DialogueSystem{
public:
    DialogueSystem(StoryFlags& flags, SDL_Renderer* renderer);
    //loads the dialogue from JSON
    void loadAllDialogue(const std::string& folderPath);

    void startDialogue(const std::string& npcId);
    void nextLine();
    void render(SDL_Renderer* renderer);
    void endDialogue();

    //now for choices
    bool choiceActive = false;
    int selectedChoice = 0;
    std::vector<Choice> currentChoices;
    bool justFinishedChoice = false;

    bool evaluteCondition(const std::string& cond);

    void jumpToLine(const std::string& id);
    bool isActive = false;


private:
    StoryFlags& storyFlags;
    std::vector<NPCDialogue> npcs;
    std::vector<dialogueLine> currentLines;
    size_t  currentIndex = 0;

    //font
    TTF_Font*font = nullptr;

    //loads file
    void loadDialogueFile(const std::string& jsonPath);


    //wrap text
    std::vector<std::string> wrapText(const std::string& text, int maxWidth);
    void renderText(SDL_Renderer* renderer, const std::string& text, int x, int y);

    //portraits
    SDL_Renderer* renderer;
    std::unordered_map<std::string, SDL_Texture*> portraitMap;
    SDL_Texture* currentPortrait = nullptr;

    //name
    std::string currentNPCName;

};

#endif //AMIPRETTY_DIALOGUESYSTEM_H
