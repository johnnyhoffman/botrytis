#ifndef UTIL_CPP
#define UTIL_CPP

#include "daisy_patch.h"
#include "daisysp.h"
#include <stdlib.h>
#include <string>

namespace util
{
    float floatMod(float a,  float b, bool absVal = false) {
        int div = (int)(a / b);
        float modded = a -= (div * b);
        return modded * (!absVal || modded > 0 ? 1 : -1);
    }
    typedef void (*Action)();
    float getVelocity(daisy::MidiEvent message)
    {
        return (float)message.data[1] / 127.f;
    }
    int getNoteNumber(daisy::MidiEvent message)
    {
        return message.data[0];
    }
    int getSemitoneFromMidiNote(int noteNumber)
    {
        return noteNumber - 69;
    }

    float getFrequencyFromSemitone(float semitone)
    {
        return powf(2, semitone / 12.f) * 440.f;
    }

    float getFrequencyFromMidiNote(int noteNumber)
    {
        return getFrequencyFromSemitone(getSemitoneFromMidiNote(noteNumber));
    }

    float getFrequency(daisy::MidiEvent message)
    {
        return getFrequencyFromMidiNote(getNoteNumber(message));
    }

    float triangle(float p)
    {
        if (p <= 0.25f)
        {
            return p * 4;
        }
        else if (p <= 0.5f)
        {
            return 1 - ((p - 0.25f) * 4);
        }
        else if (p <= 0.75f)
        {
            return (p - 0.5f) * -4;
        }
        else
        {
            return -1 * (1 - (p - 0.75f) * 4);
        }
    }

    float square(float p)
    {
        return p <= 0.5f ? -1 : 1;
    }

    float step(float p)
    {
        return 
            p <= 0.125f ? 0 :
            p <= 0.375f ? 1 : 
            p <= 0.625f ? 0 : 
            p <= 0.875f ? -1 : 0;
    }

    float doubleStep(float p)
    {
        return 
            p <= 0.0625 ? 0 :
            p <= 0.1875 ? 0.5 : 
            p <= 0.3125 ? 1 : 
            p <= 0.4375 ? 0.5 : 
            p <= 0.5625 ? 0 : 
            p <= 0.6875 ? -0.5 : 
            p <= 0.8125 ? -1 : 
            p <= 0.9375 ? -0.5 : 0;
    }

    float sine(float p)
    {
        return sinf((p - 0.5) * 2.0f * PI_F);
    }

    float sawUp(float p)
    {
        float p2 = p + 0.5;
        return (p2 > 1 ? p2 - 1 : p2) * 2 - 1;
    }

    float sawDown(float p)
    {
        return -1 * sawUp(p);
    }

    float biToUni(float bi) {
        return (bi + 1) / 2;
    }

    float uniToBi(float uni) {
        return (uni * 2) - 1;
    }

    float bpmToHertz(float bpm) {
        return bpm / 60;
    }

    void SetAttack(daisysp::Adsr* adsr, float seconds)
    {
        adsr->SetTime(daisysp::ADSR_SEG_ATTACK, seconds);
    }
    void SetDecay(daisysp::Adsr* adsr, float seconds)
    {
        adsr->SetTime(daisysp::ADSR_SEG_DECAY, seconds);
    }
    void SetRelease(daisysp::Adsr* adsr, float seconds)
    {
        adsr->SetTime(daisysp::ADSR_SEG_RELEASE, seconds);
    }
    void SetSustain(daisysp::Adsr* adsr, float level)
    {
        adsr->SetSustainLevel(level);
    }
}
#endif