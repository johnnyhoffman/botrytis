#ifndef ENVELOPE_CPP
#define ENVELOPE_CPP

#include "envelope.h"
#include "modulation_source.cpp"
#include <string>
#include "menu.cpp"
#include "icons.cpp"
#include "engine.h"
#include "util.cpp"
#include "screen_node.cpp"
#include "percent_param.cpp"
#include "screen_node.h"

void Envelope::SetMenu(MenuState* menuState) {
    menuState->menuItems[0][0]->Set("gate", gateIcon, gate->screenNode);
    menuState->menuItems[0][2]->Set("attack", aIcon, attackParam->screenNode);
    menuState->menuItems[0][3]->Set("decay", dIcon, decayParam->screenNode);
    menuState->menuItems[0][4]->Set("sustain", sIcon, sustainParam->screenNode);
    menuState->menuItems[0][5]->Set("release", rIcon, releaseParam->screenNode);
}

void Envelope::SetAttack(float seconds)
{
    util::SetAttack(adsr, seconds);
}
void Envelope::SetDecay(float seconds)
{
    util::SetDecay(adsr, seconds);
}
void Envelope::SetRelease(float seconds)
{
    util::SetRelease(adsr, seconds);
}
void Envelope::SetSustain(float level)
{
    util::SetSustain(adsr, level);
}

void Envelope::Update(float bpm) {
    float seconds = 60.0f / bpm;
    SetAttack(attackParam->GetModulatedValue() * seconds);
    SetDecay(decayParam->GetModulatedValue() * seconds);
    SetRelease(releaseParam->GetModulatedValue() * seconds);
    SetSustain(sustainParam->GetModulatedValue() * seconds);
}

void Envelope::Process() {
    value = adsr->Process(gate->value);
    modulationSource->Set(value);
}

Envelope::Envelope(std::string name, Icon* icon) {
    adsr = new daisysp::Adsr();
    this->name = name;
    attackParam = new ComplexNumericParam(new PercentParam("attack", 0.1, 0, 32, 0.001, 0.1, 0.1), createZeroTo32HundredPercentModSub, createZeroTo32HundredPercentModAdd, name + " attack", aIcon);
    decayParam = new ComplexNumericParam(new PercentParam("decay", 0.1, 0, 32, 0.001, 0.1, 0.1), createZeroTo32HundredPercentModSub, createZeroTo32HundredPercentModAdd, name + " decay", dIcon);
    releaseParam = new ComplexNumericParam(new PercentParam("release", 0.5, 0, 32, 0.001, 0.1, 0.1), createZeroTo32HundredPercentModSub, createZeroTo32HundredPercentModAdd, name + " release", rIcon);
    sustainParam = new ComplexNumericParam(new PercentParam("sustain", 0.7, 0, 1, 0.001, 0.05, 0.01), createZeroToHundredPercentModSub, createZeroToHundredPercentModAdd, name + " sustain", sIcon);
    modulationSource = new ModulationSource(name, icon);
    gate = new ModulationDestination(name + " gate", gateIcon);
    screenNode = new ScreenNode(this);
}

void Envelope::Init(float sampleRate) {
    adsr->Init(sampleRate);
}

#endif