#ifndef ENGINE_H
#define ENGINE_H
#include <stdlib.h>
#include "daisysp.h"
#include "daisy_patch.h"
#include "menu.h"

class Engine: public IHasMenu
{
public:
    void AudioCallback(float **in, float **out, size_t size);
    virtual void HandleMidiMessage(daisy::MidiEvent m) = 0;
    virtual void Init(float c_sampleRate) = 0;
    virtual void End() = 0;
protected:
    virtual void AudioCallbackInit(size_t size) = 0;
    virtual void AudioCallbackPerSample() = 0;
};

#endif