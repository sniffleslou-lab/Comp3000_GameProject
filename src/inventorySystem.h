//
// Created by Lou halpin on 28/01/2026.
//

#ifndef AMIPRETTY_INVENTORYSYSTEM_H
#define AMIPRETTY_INVENTORYSYSTEM_H

#include <vector>
#include <string>
#include <unordered_map>
#include "inventoryItem.h"

class inventorySystem {
public:
    inventorySystem() = default;

    void addItem(const std::string& id);
    void removeItem(const std::string& id);

    bool hasItem(const std::string& id) const;

    //now for registering
    void registerItem(const inventoryItem& item);
    const inventoryItem* getItemDefine(const std::string& id) const;

private:
    std::vector<std::string> items;//item id
    std::unordered_map<std::string, inventoryItem> define;

};

#endif //AMIPRETTY_INVENTORYSYSTEM_H
