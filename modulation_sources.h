#ifndef MODULATION_SOURCES_H
#define MODULATION_SOURCES_H

#include "modulation_source.h"
#include <string>

class ModulationSources : public IHasMenu
{
public:
    ModulationSource* ctrls[4];

    ModulationSource* audioIns[4];

    ModulationSource* gateIns[2];

    ModulationSource* globalModulationSources[MENU_WIDTH * 2];
    ModulationSource* engineModulationSources[MENU_WIDTH * 2];

    ScreenNode *screenNode = nullptr;

    void SetMenu(MenuState* menuState);

    ModulationSources();
};
ModulationSources modulationSources;

void SetGlobalModulationSources()
{
    for (int i = 0; i< 4; i ++) {
        modulationSources.globalModulationSources[i] = modulationSources.ctrls[i];
    }
    modulationSources.globalModulationSources[4] = 0;
    for (int i = 0; i < 4; i++)
    {
        modulationSources.globalModulationSources[i + 5] = modulationSources.audioIns[i];
    }
    modulationSources.globalModulationSources[9] = 0;

    for (int i = 0; i < 2; i ++) {
        modulationSources.globalModulationSources[i + 10] = modulationSources.gateIns[i];
    }
}
#endif