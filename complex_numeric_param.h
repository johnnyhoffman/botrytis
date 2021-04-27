#ifndef COMPLEX_NUMERIC_PARAM_H
#define COMPLEX_NUMERIC_PARAM_H

#include "numeric_param.h"
#include "modulation_source.h"
#include <string>
#include "menu.h"
#include "engine.h"
#include "screen_node.h"
#include "icons.cpp"

class ComplexNumericParam : public IHasMenu
{
public:
    NumericParam* baseValue = nullptr;
    NumericParamFactory modulationSubtractFactory = nullptr;
    NumericParamFactory modulationAddFactory = nullptr;
    ModulationDestination* modulationDestinations[8] = {nullptr, nullptr, nullptr, nullptr};
    ScreenNode *screenNode = nullptr;
    float GetModulatedValue();
    float GetUnmodulatedValue();
    ComplexNumericParam(NumericParam* baseValue, NumericParamFactory modulationAddFactory, NumericParamFactory modulationSubtractFactory, std::string label, Icon* icon);
    void SetMenu(MenuState* menuState);
};

#endif