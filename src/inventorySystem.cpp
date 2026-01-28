#include "inventorySystem.h"
#include <algorithm>

void inventorySystem::addItem(const std::string &id) {
    items.push_back(id);
}
void inventorySystem::removeItem(const std::string &id) {
    items.erase(std::remove(items.begin(),items.end(),id),items.end());
}
bool inventorySystem::hasItem(const std::string &id) const {
    return std::find(items.begin(),items.end(),id) != items.end();
}
void inventorySystem::registerItem(const inventoryItem &item) {
    define[item.id]=item;
}
const inventoryItem* inventorySystem::getItemDefine(const std::string &id) const {
    auto it = define.find(id);
    if (it != define.end()){
        return &it->second;
    }
    return nullptr;
}

