//
// Created by Lou halpin on 17/11/2025.
//
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#include "dialogueSystem.h"
#include "filesystem"

DialogueSystem::DialogueSystem(StoryFlags &flags)
: storyFlags(flags) {}

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
    if (currentIndex<currentLines.size()){
        std::cout<<currentLines[currentIndex].text<<std::endl;
    }
}

