
#ifndef GLOBALS_CPP
#define GLOBALS_CPP
#include "modulation_destination.h"
#include "modulation_source.h"
#include "engine.h"
#include "daisy_patch.h"
#include "daisysp.h"
#include "screen_node.h"

using namespace daisy;

class Globals
{
public:
    DaisyPatch patch;
    ModulationDestination* modulationDestinationForMenu = nullptr;
    ModulationSource* modulationSourceForMenu = nullptr;
    Engine* engine = nullptr;
    ModulationDestination *audioOutsModulationDestinations[4] = {nullptr, nullptr, nullptr, nullptr};
    ModulationDestination *gateOutModulationDestination = nullptr;
    ModulationDestination *cvOut1ModulationDestination = nullptr;
    ModulationDestination *cvOut2ModulationDestination = nullptr;
    ScreenNode *emptyModSourceScreenNode = nullptr;
};

Globals globals;

#endif