//
// Created by Lou halpin on 17/11/2025.
//
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#include "dialogueSystem.h"
#include "filesystem"

DialogueSystem::DialogueSystem(StoryFlags &flags)
: storyFlags(flags) {
    font = TTF_OpenFont("../assets/font/Sunlight Dreams.otf",24);
    if(!font){
        std::cerr << "failed to load font: " << TTF_GetError()<<std::endl;
    }
}

void DialogueSystem::loadDialogueFile(const std::string &jsonPath) {
    std::ifstream file(jsonPath);
    if (!file.is_open()){
        std::cerr << "could not open " << jsonPath;
        return;
    }
    nlohmann::json data;
    file >> data;
    std::cout<<"JSON File \" " << jsonPath << "\" is loaded. \n";

    NPCDialogue npc;
    npc.npcId = data["npc"];

    for (auto& lineJson : data["dialogue"]){
        dialogueLine line;
        line.id = lineJson["id"];
        line.text = lineJson["text"];
        if (lineJson.contains("condition")){
            line.condition = lineJson["condition"];
        }
        npc.lines.push_back(line);
    }
    npcs.push_back(npc);
}
//this checks that it can load the file. going to load the files in main
void DialogueSystem::loadAllDialogue(const std::string &folderPath) {
    for (const auto& entry : std::filesystem::directory_iterator(folderPath)){
        if (entry.path().extension()==".json"){
            loadDialogueFile(entry.path().string());
        }
    }
}
void DialogueSystem::startDialogue(const std::string &npcId) {
    currentLines.clear();
    currentIndex = 0;
    for (auto& npc : npcs) {
        if(npc.npcId == npcId){
            for (auto& line : npc.lines){
                if (line.condition.empty() || storyFlags.getFlag(line.condition)){
                    currentLines.push_back(line);
                }
            }
            break;
        }


    }
}
void DialogueSystem::nextLine() {
    if (currentIndex < currentLines.size()){
        currentIndex++;
    }
}
void DialogueSystem::render(SDL_Renderer *renderer) {
   /* if (currentIndex<currentLines.size()){
        std::cout<<currentLines[currentIndex].text<<std::endl;
    }*/
    if (currentLines.empty())return;

    //now we are drawing
    SDL_Rect box={50,400,700,150};
    SDL_SetRenderDrawColor(renderer, 0,0,0,200);
    SDL_RenderFillRect(renderer, &box);

    //now we redner the lines
    const dialogueLine& line = currentLines[currentIndex];
    SDL_Color white = {255,255,255,255};
    SDL_Surface* surface = TTF_RenderText_Blended(font,line.text.c_str(),white);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer,surface);

    SDL_Rect dst = {70,420, surface->w, surface->h};
    SDL_RenderCopy(renderer,texture, nullptr,&dst);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);


}

