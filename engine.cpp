#ifndef ENGINE_CPP
#define ENGINE_CPP
#include <stdlib.h>
#include "daisysp.h"
#include "daisy_patch.h"
#include "engine.h"
#include "modulation_sources.h"
#include "util.cpp"
#include "globals.cpp"

using namespace daisy;

void Engine::AudioCallback(float **in, float **out, size_t size) {
    AudioCallbackInit(size);

    for (unsigned int n = 0; n < size; n++)
    {
        modulationSources.audioIns[0]->Set(util::biToUni(in[0][n]));
        modulationSources.audioIns[1]->Set(util::biToUni(in[1][n]));
        modulationSources.audioIns[2]->Set(util::biToUni(in[2][n]));
        modulationSources.audioIns[3]->Set(util::biToUni(in[3][n]));

        AudioCallbackPerSample();
        for (int i = 0; i < 4; i++) {
            out[i][n] = globals.audioOutsModulationDestinations[i]->value > 1 ? 1 : globals.audioOutsModulationDestinations[i]->value < -1 ? -1 : globals.audioOutsModulationDestinations[i]->value;
        }
        dsy_gpio_write(&globals.patch.gate_output, globals.gateOutModulationDestination->value ? 1 : 0);
        
        globals.patch.seed.dac.WriteValue(
            DacHandle::Channel::ONE,
            4095 * (globals.cvOut1ModulationDestination->value < 0
                ? 0
                : globals.cvOut1ModulationDestination->value > 1
                    ? 1
                    : globals.cvOut1ModulationDestination->value));

        globals.patch.seed.dac.WriteValue(
            daisy::DacHandle::Channel::TWO,
            4095 * (globals.cvOut2ModulationDestination->value < 0
                ? 0
                : globals.cvOut2ModulationDestination->value > 1
                    ? 1
                    : globals.cvOut2ModulationDestination->value));
    }
}
#endif