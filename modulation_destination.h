#ifndef MODULATION_DESTINATION_H
#define MODULATION_DESTINATION_H

#include "menu.h"
#include "numeric_param.h"
#include "engine.h"
#include "screen_node.h"
#include "icons.cpp"
#include <string>

class ModulationSource;
class ModulationDestination : public IHasMenu
{
public:
    std::string label;
    Icon* icon;
    NumericParam* modulationSubtract = nullptr;
    NumericParam* modulationAdd = nullptr;
    ModulationDestination* multiplier = nullptr;
    ScreenNode *screenNode = nullptr;
    float value;
    ModulationSource* source;
    void Clear();
    float GetMultipliedValue();
    ModulationDestination();
    ModulationDestination(std::string label, Icon* icon);
    ModulationDestination(NumericParam* modulationSubtract, NumericParam* modulationAdd, std::string label, Icon* icon);
    void SetMenu(MenuState* menuState);
    void InitializeScreenNodes();
};

#endif