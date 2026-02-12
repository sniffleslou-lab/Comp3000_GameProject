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

struct dialogueLine{
    std::string id;
    std::string text;
    std::string condition;
};
struct NPCDialogue{
    std::string npcId;
    std::vector<dialogueLine> lines;
};

class DialogueSystem{
public:
    DialogueSystem(StoryFlags& flags);
    //loads the dialogue from JSON
    void loadAllDialogue(const std::string& folderPath);

    void startDialogue(const std::string& npcId);
    void nextLine();
    void render(SDL_Renderer* renderer);
    void endDialogue();

private:
    StoryFlags& storyFlags;
    std::vector<NPCDialogue> npcs;
    std::vector<dialogueLine> currentLines;
    size_t  currentIndex = 0;

    //font
    TTF_Font*font = nullptr;

    //loads file
    void loadDialogueFile(const std::string& jsonPath);

    bool isActive = false;

    //wrap text
    std::vector<std::string> wrapText(const std::string& text, int maxWidth);

};

#endif //AMIPRETTY_DIALOGUESYSTEM_H
