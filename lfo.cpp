#ifndef LFO_CPP
#define LFO_CPP

#include "lfo.h"
#include "modulation_source.cpp"
#include <string>
#include "menu.cpp"
#include "icons.cpp"
#include "engine.h"
#include "screen_node.cpp"
#include "wave_function.cpp"
#include "float_param.cpp"
#include "wave_morph_param.cpp"
#include "percent_param.cpp"

void LFO::SetMenu(MenuState* menuState) {
    menuState->menuItems[0][0]->Set("period", periodIcon, periodParam->screenNode);
    menuState->menuItems[0][1]->Set("phase shift", phIcon, phaseShiftParam->screenNode);
    menuState->menuItems[0][2]->Set("gate", gateIcon, gate->screenNode);
    menuState->menuItems[0][3]->Set("wave morph", waveIcon, waveMorphParam->screenNode);
    menuState->menuItems[0][4]->Set(biPolar->value ? "is bi-polar" : "is uni-polar", biPolar->value ? bipolarIcon : unipolarIcon, biPolar->screenNode);
}

void LFO::Update() {

}

void LFO::Process(float bpm) {
    float seconds = 60.0f / bpm;
    phase += periodParam->GetModulatedValue() * seconds * (1.0 / samplerate);
    if (phase >= 1.0f) {
        phase -= 1.0f;
    }
    if (gate->value && gateReset) {
        gateReset = false;
        phase = 0;
    }
    if (!gate->value) {
        gateReset = true;
    }
    value = lfoWaves(waveMorphParam->value, phase + phaseShiftParam->GetModulatedValue());
    if (!biPolar->value) {
        value = util::uniToBi(value);
    }
    modulationSource->Set(value);
}

LFO::LFO(std::string name, Icon* icon) {
    this->name = name;
    phaseShiftParam = new ComplexNumericParam(new PercentParam("phase shift", 0, 0, 1, 0.001, 0.1, 0.05), createZeroToHundredPercentModSub, createZeroToHundredPercentModAdd, name + " phase shift", phIcon);
    waveMorphParam = new WaveMorphParam(lfoWaves, "wave morph", 0, 0, 1, 0.001, 0.1, 0.22, phaseShiftParam->baseValue);
    periodParam = new ComplexNumericParam(new PercentParam("period", 1, 0.001, 1000, 0.001, 0.25, 0.25), createZeroToHundredPercentModSub, createZeroToHundredPercentModAdd, name + " period", periodIcon);
    modulationSource = new ModulationSource(name, icon);
    gate = new ModulationDestination(name + " gate", gateIcon);
    phase = 0;
    biPolar = new BoolParam();
    biPolar->value = true;
    screenNode = new ScreenNode(this);
}

void LFO::Init(float samplerate) {
    this->samplerate = samplerate;
}

#endif