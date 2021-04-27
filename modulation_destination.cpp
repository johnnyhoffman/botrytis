


#ifndef MODULATION_DESTINATION_CPP
#define MODULATION_DESTINATION_CPP

#include "modulation_destination.h"
#include <string>
#include "menu.cpp"
#include "icons.cpp"
#include "modulation_sources.h"
#include "engine.h"

void ModulationDestination::SetMenu(MenuState* menuState)
{
    for (int i = 0; i < MENU_WIDTH; i++) {
        if (modulationSources.globalModulationSources[i]) {
            menuState->menuItems[1][i]->Set(
                modulationSources.globalModulationSources[i]->label,
                source == modulationSources.globalModulationSources[i] ? modulationSources.globalModulationSources[i]->invertedIcon : modulationSources.globalModulationSources[i]->icon,
                modulationSources.globalModulationSources[i]->screenNode);
        }
        if (modulationSources.globalModulationSources[i + MENU_WIDTH]) {
            menuState->menuItems[2][i]->Set(
                modulationSources.globalModulationSources[i + MENU_WIDTH]->label, 
                source == modulationSources.globalModulationSources[i + MENU_WIDTH] ? modulationSources.globalModulationSources[i + MENU_WIDTH]->invertedIcon : modulationSources.globalModulationSources[i + MENU_WIDTH]->icon, 
                modulationSources.globalModulationSources[i + MENU_WIDTH]->screenNode);
        }
    }

    for (int i = 0; i < MENU_WIDTH; i++) {
        if (modulationSources.engineModulationSources[i]) {
            menuState->menuItems[3][i]->Set(
                modulationSources.engineModulationSources[i]->label,
                source == modulationSources.engineModulationSources[i] ? modulationSources.engineModulationSources[i]->invertedIcon : modulationSources.engineModulationSources[i]->icon,
                modulationSources.engineModulationSources[i]->screenNode);
        }
        if (modulationSources.engineModulationSources[i + MENU_WIDTH]) {
            menuState->menuItems[4][i]->Set(
                modulationSources.engineModulationSources[i + MENU_WIDTH]->label,
                source == modulationSources.engineModulationSources[i + MENU_WIDTH] ? modulationSources.engineModulationSources[i + MENU_WIDTH]->invertedIcon : modulationSources.engineModulationSources[i + MENU_WIDTH]->icon,
                modulationSources.engineModulationSources[i + MENU_WIDTH]->screenNode);
        }
    }

    if (modulationSubtract) {
        menuState->menuItems[0][0]->Set("mod subtract", minusIcon, modulationSubtract->screenNode);
    }
    if (modulationAdd) {
        menuState->menuItems[0][1]->Set("mod add", plusIcon, modulationAdd->screenNode);
    }
    if (multiplier) {
        menuState->menuItems[0][2]->Set("multiplier", xIcon, multiplier->screenNode);
    }
    menuState->menuItems[MENU_HEIGHT - 1][0]->Set("none", croosedCircleIcon, globals.emptyModSourceScreenNode);
}

void ModulationDestinationEnter(ScreenNode* screenNode) {
    globals.modulationDestinationForMenu = (ModulationDestination*)screenNode->iHasMenu;
}

void ModulationDestinationBack(ScreenNode* screenNode) {
    globals.modulationDestinationForMenu = nullptr;
}

ModulationDestination::ModulationDestination(NumericParam* modulationSubtract, NumericParam* modulationAdd, std::string label, Icon* icon) {
    this->label = label;
    this->icon = icon;

    source = nullptr;
    value = 0;
    this->modulationSubtract = modulationSubtract;
    this->modulationAdd = modulationAdd;
    multiplier = new ModulationDestination(label + " multiplier", icon);

    screenNode = new ScreenNode(this);
    screenNode->enter = ModulationDestinationEnter;
    screenNode->back = ModulationDestinationBack;
}

ModulationDestination::ModulationDestination(std::string label, Icon* icon) {
    this->label = label;
    this->icon = icon;
    value = 0;
    source = nullptr;

    screenNode = new ScreenNode(this);
    screenNode->enter = ModulationDestinationEnter;
    screenNode->back = ModulationDestinationBack;
}

void ModulationDestination::Clear() {
    if (source) {
        source->destinations.remove(this);
        source = nullptr;
    }
    value = 0;
}

float ModulationDestination::GetMultipliedValue() {
    return value * (multiplier && multiplier->source ? multiplier->value : 1);
}

#endif