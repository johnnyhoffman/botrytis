#ifndef COMPLEX_NUMERIC_PARAM_CPP
#define COMPLEX_NUMERIC_PARAM_CPP

#include "complex_numeric_param.h"
#include "numeric_param.cpp"
#include "percent_param.cpp"
#include "float_param.cpp"
#include "modulation_source.cpp"
#include <string>
#include "menu.cpp"
#include "icons.cpp"
#include "engine.h"
#include "screen_node.cpp"

float ComplexNumericParam::GetModulatedValue()
{
    float value = GetUnmodulatedValue();
        
    for (int i = 0; i < 4 ; i++)
    {
        if (!modulationDestinations[i]->source) continue;
        float modulationSubtractValue = modulationDestinations[i]->modulationSubtract ? modulationDestinations[i]->modulationSubtract->GetValue() : 0;
        float modulationAddValue = modulationDestinations[i]->modulationAdd ? modulationDestinations[i]->modulationAdd->GetValue() : 1;
        value -= modulationSubtractValue;
        float modulationRange = modulationAddValue + modulationSubtractValue;
        value += modulationDestinations[i]->GetMultipliedValue() * modulationRange;
    }
    // TODO scale logarithmically or by a generic graph
    return value;
}
float ComplexNumericParam::GetUnmodulatedValue()
{
    return baseValue->GetValue();
}

void ComplexNumericParam::SetMenu(MenuState* menuState) {
    menuState->menuItems[0][0]->Set("base value", percentIcon, baseValue->screenNode);
    menuState->menuItems[1][0]->Set("modulation source 1", !!modulationDestinations[0]->source ? modulationDestinations[0]->source->icon : modIcon, modulationDestinations[0]->screenNode);
    menuState->menuItems[1][1]->Set("modulation source 2", !!modulationDestinations[1]->source ? modulationDestinations[1]->source->icon : modIcon, modulationDestinations[1]->screenNode);
    menuState->menuItems[1][2]->Set("modulation source 3", !!modulationDestinations[2]->source ? modulationDestinations[2]->source->icon : modIcon, modulationDestinations[2]->screenNode);
    menuState->menuItems[1][3]->Set("modulation source 4", !!modulationDestinations[3]->source ? modulationDestinations[3]->source->icon : modIcon, modulationDestinations[3]->screenNode);
}

ComplexNumericParam::ComplexNumericParam(NumericParam* baseValue, NumericParamFactory modulationSubtractFactory, NumericParamFactory modulationAddFactory, std::string label, Icon* icon)
{
    this->baseValue = baseValue;
    this->modulationAddFactory = modulationAddFactory;
    this->modulationSubtractFactory = modulationSubtractFactory;
    
    for (int i = 0; i < 4 ; i++)
    {
        this->modulationDestinations[i] = new ModulationDestination(modulationSubtractFactory(), modulationAddFactory(), label + " " + std::to_string(i), icon);
    }

    this->screenNode = new ScreenNode(this);
}

NumericParam* createDetuneModSub() {
    return new FloatParam("mod sub", 1, -88, 88, 0.01, 1, 1);
}
NumericParam* createDetuneModAdd() {
    return new FloatParam("mod add", 1, -88, 88, 0.01, 1, 1);
}
NumericParam* createBPMModSub() {
    return new FloatParam("mod sub", 1, 0, 60, 0.01, 1, 1);
}
NumericParam* createBPMModAdd() {
    return new FloatParam("mod add", 1, 0, 60, 0.01, 1, 1);
}
NumericParam* createNoteNumberModSub() {
    return new FloatParam("mod sub", 1, 0, 60, 0.01, 1, 1);
}
NumericParam* createNoteNumberModAdd() {
    return new FloatParam("mod add", 1, 0, 60, 0.01, 1, 1);
}
NumericParam* createDivisionModSub() {
    return new FloatParam("mod sub", 4, 0, 8, 0.01, 1, 1);
}
NumericParam* createDivisionModAdd() {
    return new FloatParam("mod add", 4, 0, 8, 0.01, 1, 1);
}
NumericParam* createRawFreqModSub() {
    return new FloatParam("mod sub", 2000, 0, 40000, 0.01, 50, 50);
}
NumericParam* createRawFreqModAdd() {
    return new FloatParam("mod add", 2000, 0, 40000, 0.01, 50, 50);
}
NumericParam* createZeroToHundredPercentModSub() {
    return new PercentParam("mod sub", 0, 0, 1, 0.001, 0.05, 0.01);
}
NumericParam* createZeroToHundredPercentModAdd() {
    return new PercentParam("mod add", 1, 0, 1, 0.001, 0.05, 0.01);
}
NumericParam* createZeroTo32HundredPercentModSub() {
    return new PercentParam("mod sub", 0, 0, 32, 0.001, 0.2, 0.01);
}
NumericParam* createZeroTo32HundredPercentModAdd() {
    return new PercentParam("mod add", 1, 0, 32, 0.001, 0.2, 0.01);
}
NumericParam* createFreqModSub() {
    return new PercentParam("mod sub", 0.25, 0, 8, 0.001, 0.05, 0.01);
}
NumericParam* createFreqModAdd() {
    return new PercentParam("mod add", 0.5, 0, 8, 0.001, 0.05, 0.01);
}
NumericParam* createSustainModSub() {
    return new PercentParam("sus mod sub", 0.2, 0, 1, 0.001, 0.05, 0.01);
}
NumericParam* createSustainModAdd() {
    return new PercentParam("sus mod add", 0.2, 0, 1, 0.001, 0.05, 0.01);
}

#endif