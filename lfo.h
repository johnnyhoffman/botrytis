#ifndef LFO_H
#define LFO_H

#include "complex_numeric_param.h"
#include "numeric_param.h"
#include "bool_param.h"
#include "modulation_source.h"
#include <string>
#include "menu.h"
#include "engine.h"


class LFO : public IHasMenu
{
public:
    std::string name;
    void SetMenu(MenuState* menuState);
    ModulationSource* modulationSource;
    LFO(std::string name, Icon* icon);
    void Init(float samplerate);
    void Update();
    void Process(float bpm);
    float value = 0;
    float samplerate;
    float phase = 0;
    bool gateReset = true;
    ModulationDestination* gate = nullptr;
    BoolParam* biPolar = nullptr;
    ScreenNode *screenNode = nullptr;
    ComplexNumericParam* periodParam = nullptr;
    ComplexNumericParam* phaseShiftParam = nullptr;
    NumericParam* waveMorphParam = nullptr;
};

#endif