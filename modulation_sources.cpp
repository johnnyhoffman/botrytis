#ifndef MODULATION_SOURCES_CPP
#define MODULATION_SOURCES_CPP

#include "modulation_source.h"
#include "modulation_sources.h"
#include <list>
#include <string>
#include "icons.cpp"
#include "menu.cpp"
#include "globals.cpp"

ModulationSources::ModulationSources() {
    ctrls[0] = new ModulationSource("ctrl 1", ctrl1Icon);
    ctrls[1] = new ModulationSource("ctrl 2", ctrl2Icon);
    ctrls[2] = new ModulationSource("ctrl 3", ctrl3Icon);
    ctrls[3] = new ModulationSource("ctrl 4", ctrl4Icon);

    audioIns[0] = new ModulationSource("audio in 1", audioIn1Icon);
    audioIns[1] = new ModulationSource("audio in 2", audioIn2Icon);
    audioIns[2] = new ModulationSource("audio in 3", audioIn3Icon);
    audioIns[3] = new ModulationSource("audio in 4", audioIn4Icon);

    gateIns[0] = new ModulationSource("gate in 1", gate1Icon);
    gateIns[1] = new ModulationSource("gate in 2", gate2Icon);

    screenNode = new ScreenNode(this);
}

void ModulationSources::SetMenu(MenuState* menuState)
{
    for (int i = 0; i < MENU_WIDTH; i++) {
        if (modulationSources.globalModulationSources[i] && modulationSources.globalModulationSources[i]->destinations.size()) {
            menuState->menuItems[0][i]->Set(modulationSources.globalModulationSources[i]->label, modulationSources.globalModulationSources[i]->icon, modulationSources.globalModulationSources[i]->screenNode);
        }
        if (modulationSources.globalModulationSources[i + MENU_WIDTH] && modulationSources.globalModulationSources[i + MENU_WIDTH]->destinations.size()) {
            menuState->menuItems[1][i]->Set(modulationSources.globalModulationSources[i + MENU_WIDTH]->label, modulationSources.globalModulationSources[i + MENU_WIDTH]->icon, modulationSources.globalModulationSources[i + MENU_WIDTH]->screenNode);
        }
    }

    for (int i = 0; i < MENU_WIDTH; i++) {
        if (modulationSources.engineModulationSources[i] && modulationSources.engineModulationSources[i]->destinations.size()) {
            menuState->menuItems[2][i]->Set(modulationSources.engineModulationSources[i]->label, modulationSources.engineModulationSources[i]->icon, modulationSources.engineModulationSources[i]->screenNode);
        }
        if (modulationSources.engineModulationSources[i + MENU_WIDTH] && modulationSources.engineModulationSources[i + MENU_WIDTH]->destinations.size()) {
            menuState->menuItems[3][i]->Set(modulationSources.engineModulationSources[i + MENU_WIDTH]->label, modulationSources.engineModulationSources[i + MENU_WIDTH]->icon, modulationSources.engineModulationSources[i + MENU_WIDTH]->screenNode);
        }
    }
}

#endif