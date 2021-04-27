#ifndef ENVELOPE_H
#define ENVELOPE_H

#include "complex_numeric_param.h"
#include "numeric_param.h"
#include "modulation_source.h"
#include <string>
#include "menu.h"
#include "engine.h"


class Envelope : public IHasMenu
{
public:
    std::string name;
    daisysp::Adsr* adsr = nullptr;
    void SetMenu(MenuState* menuState);
    ModulationSource* modulationSource;
    Envelope(std::string name, Icon* icon);
    void Init(float sampleRate);
    void Update(float bpm);
    void Process();
    float value = 0;
    ModulationDestination* gate = nullptr;
    ScreenNode *screenNode = nullptr;
    ComplexNumericParam* attackParam = nullptr;
    ComplexNumericParam* decayParam = nullptr;
    ComplexNumericParam* releaseParam = nullptr;
    ComplexNumericParam* sustainParam = nullptr;
private:
    void SetAttack(float seconds);
    void SetDecay(float seconds);
    void SetRelease(float seconds);
    void SetSustain(float level);
};

#endif