#ifndef MODULATION_SOURCE_H
#define MODULATION_SOURCE_H

#include <string>
#include <list>
#include "modulation_destination.h"

class ModulationSource: public IHasMenu
{
public:
    std::string label;
    Icon* icon;
    Icon* invertedIcon;
    void Set(float value);
    void AssignDestination(ModulationDestination *destination);
    ModulationSource(std::string label, Icon* icon);
    void SetMenu(MenuState* menuState);
    std::list<ModulationDestination *> destinations;
    ScreenNode *screenNode = nullptr;

private:
    float value;
};

#endif
