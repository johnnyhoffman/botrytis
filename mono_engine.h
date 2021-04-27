#ifndef MONO_ENGINE_H
#define MONO_ENGINE_H

#include "engine.h"
#ifdef ENABLE_PRESET_DEVELOPMENT
#include "presets.h"
#endif
#include "lfo.h"
#include "wave_function.cpp"
#include "numeric_param.cpp"
#include "complex_numeric_param.cpp"
#include "float_param.cpp"
#include "percent_param.cpp"
#include "bool_param.h"
#include "envelope.h"
#include <list>
#include "daisysp.h"
#include "screen_node.h"

using namespace daisy;

class MonoEngineClock: IHasMenu {
    public:
        float phase = 0;
        bool inResetGate = false;
        ModulationDestination *resetGate = nullptr;
        ComplexNumericParam* bpmParam = nullptr;
        ComplexNumericParam* divisionParam = nullptr;
        ModulationSource* asModSource = nullptr;
        BoolParam* invert = nullptr;
        void SetMenu(MenuState* menuState);
        ScreenNode *screenNode = nullptr;
        MonoEngineClock();
};

class MonoEngineOscillator: IHasMenu {
    public:
        WaveFunction waveFunction; 
        float phase;
        float glideSourceSemitone;
        float glideDestinationSemitone;
        float glideRemainingProportion;
        bool glideReset = true;
        ComplexNumericParam* levelParam = nullptr;
        NumericParam* waveMorphParam = nullptr;
        ComplexNumericParam* glideParam = nullptr;
        ModulationDestination* glideGate = nullptr;
        ComplexNumericParam* phaseShiftParam = nullptr;
        ComplexNumericParam* detuneSemitoneParam = nullptr;
        ModulationSource* asModSource = nullptr;
        void SetMenu(MenuState* menuState);
        ScreenNode *screenNode = nullptr;
        MonoEngineOscillator();
};

class MonoEngineFilter: IHasMenu {
    public:
        ModulationDestination* input = nullptr;
        ComplexNumericParam* filterFreqKeyTrackParam = nullptr;
        ComplexNumericParam* filterResonanceParam = nullptr;
        ComplexNumericParam* glideParam = nullptr;
        ModulationDestination* glideGate = nullptr;
        BoolParam* filterOn = nullptr;
        daisysp::MoogLadder* moogLadder = nullptr;
        ModulationSource* asModSource = nullptr;
        float glideSourceSemitone;
        float glideDestinationSemitone;
        float glideRemainingProportion;
        bool glideReset = true;
        void SetMenu(MenuState* menuState);
        ScreenNode *screenNode = nullptr;
        MonoEngineFilter();
};

class MonoEngineDelay: IHasMenu {
    public:
        ModulationDestination* input = nullptr;
        BoolParam* delayOn = nullptr;
        daisysp::DelayLine<float, 36000> *delayLine = nullptr;
        ComplexNumericParam* levelParam = nullptr;
        ModulationSource *asModSource = nullptr;
        ComplexNumericParam* feedbackParam = nullptr;
        ComplexNumericParam* delayTimeParam = nullptr;
        void SetMenu(MenuState* menuState);
        ScreenNode *screenNode = nullptr;
        MonoEngineDelay();
};

class MonoEngineMixer: IHasMenu {
    public:
        ComplexNumericParam* masterLevel = nullptr;
        ModulationDestination* input1 = nullptr;
        ComplexNumericParam* input1Level = nullptr;
        ModulationDestination* input2 = nullptr;
        ComplexNumericParam* input2Level = nullptr;
        ModulationDestination* input3 = nullptr;
        ComplexNumericParam* input3Level = nullptr;
        ModulationDestination* input4 = nullptr;
        ComplexNumericParam* input4Level = nullptr;
        ModulationSource *asModSource = nullptr;
        void SetMenu(MenuState* menuState);
        ScreenNode *screenNode = nullptr;
        MonoEngineMixer();
};

class MonoEnginePhaser: IHasMenu {
    public:
        ModulationDestination* input = nullptr;
        BoolParam* phaserOn = nullptr;
        daisysp::Phaser *phaser = nullptr;
        ComplexNumericParam* levelParam = nullptr;
        ComplexNumericParam* frequencyParam = nullptr;
        ComplexNumericParam* lfoDepthParam = nullptr;
        ComplexNumericParam* lfoFrequencyParam = nullptr;
        ComplexNumericParam* feedbackParam = nullptr;
        ModulationSource *asModSource = nullptr;
        void SetMenu(MenuState* menuState);
        ScreenNode *screenNode = nullptr;
        MonoEnginePhaser();
};

class MonoEngine : public Engine
{
public:
    #ifdef ENABLE_PRESET_DEVELOPMENT
    Presets* presets;
    #endif
    MonoEngineClock clock;
    MonoEngineOscillator oscillators[3];
    MonoEngineFilter filter;
    MonoEngineDelay delay;
    MonoEnginePhaser phaser;
    MonoEngineMixer mixers[2];
    void AudioCallbackInit(size_t size);
    void AudioCallbackPerSample();
    void HandleMidiMessage(MidiEvent m);
    void ProcessClock(float bpm);
    void ProcessOscillators();
    void ProcessFilter();
    #ifndef DISABLE_PHASER
    void ProcessPhaser();
    #endif
    void ProcessDelay();
    void ProcessMixers();
    void Init(float c_sampleRate);
    void End();
    void PreWire();
    MonoEngine();
    void SetMenu(MenuState* menuState);
    Envelope* envelopes[5];
    LFO* lfos[2];
    ModulationSource* gate = nullptr;
    ModulationSource* velocity = nullptr;
    float velocityState = 0;
    ModulationSource* channelPressure = nullptr;
    ScreenNode *screenNode = nullptr;

private:
    int semitone;
    int lastSemitone;
    unsigned int lastActiveNotesCount;
    struct MonoActiveNote
    {
        int noteNumber;
        float velocity;
    };
    float sampleRate;
    std::list<MonoActiveNote> activeNotes;

    void Noteon(int noteNumber, float velocity);
    void Noteoff(int noteNumber);
};
#endif
