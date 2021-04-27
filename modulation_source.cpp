#ifndef MODULATION_SOURCE_CPP
#define MODULATION_SOURCE_CPP

#include <string>
#include "modulation_source.h"
#include "screen_node.cpp"
#include "modulation_destination.h"
#include "percent_param.cpp"
#include "globals.cpp"
#include "engines.h"

void ModulationSource::Set(float value)
{
    this->value = value;
    for (std::list<ModulationDestination *>::iterator it = this->destinations.begin(); it != this->destinations.end(); it++)
    {
        (*it)->value = value;
    }
}
void ModulationSource::AssignDestination(ModulationDestination *destination)
{
    if (destination->source) {
        destination->source->destinations.remove(destination);
    }
    this->destinations.push_back(destination);
    destination->source = this;
    destination->value = this->value;
}


void ModulationSourceEnter(ScreenNode* screenNode) {
    globals.modulationSourceForMenu = (ModulationSource*)screenNode->iHasMenu;
}

void ModulationSourceBack(ScreenNode* screenNode) {
    globals.modulationSourceForMenu = nullptr;
}

ModulationSource::ModulationSource(std::string label, Icon* icon) {
    this->label = label;
    this->icon = icon;
    this->invertedIcon = new Icon(icon, true);
    screenNode = new ScreenNode(this, ModulationSourceScreenNode);
    screenNode->enter = ModulationSourceEnter;
    screenNode->back = ModulationSourceBack;
}

void ModulationSource::SetMenu(MenuState* menuState)
{
    auto iterator = destinations.begin();
    for (int y = 0; y < MENU_HEIGHT; y++) {
        for (int x = 0; x < MENU_WIDTH; x++) {
            if (iterator != destinations.end()) {
                if (*iterator)
                {
                    menuState->menuItems[y][x]->Set((*iterator)->label, (*iterator)->icon, (*iterator)->screenNode);
                }
                iterator++;
            }
        }
    }
}

#endif