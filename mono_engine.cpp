#ifndef MONO_ENGINE_CPP
#define MONO_ENGINE_CPP

#include "mono_engine.h"
#include "screen_node.h"
#include "menu.h"
#include "engine.h"
#include "engines.h"
#include "daisy_patch.h"
#include "daisysp.h"
#include "util.cpp"
#include <stdlib.h>
#include <list>
#include "numeric_param.cpp"
#include "bool_param.cpp"
#include "modulation_sources.h"
#include "wave_morph_param.cpp"
#include "percent_param.cpp"
#include "screen_node.cpp"
#include "wave_function.cpp"
#include "envelope.cpp"
#ifdef ENABLE_PRESET_DEVELOPMENT
#include "presets.cpp"
#endif
#include "lfo.cpp"

using namespace daisy;

void MonoEngineOscillator::SetMenu(MenuState* menuState)
{
    menuState->menuItems[0][0]->Set("level", lIcon, levelParam->screenNode);
    menuState->menuItems[0][2]->Set("wave morph", waveIcon, waveMorphParam->screenNode);
    menuState->menuItems[0][3]->Set("detune", dtIcon, detuneSemitoneParam->screenNode);
    menuState->menuItems[0][4]->Set("phase shift", phIcon, phaseShiftParam->screenNode);
    menuState->menuItems[0][6]->Set("glide", glideIcon, glideParam->screenNode);
    menuState->menuItems[0][7]->Set("glide gate", glideIcon, glideGate->screenNode);
}

void MonoEngineFilter::SetMenu(MenuState* menuState)
{
    menuState->menuItems[0][0]->Set("source", sIcon, input->screenNode);
    menuState->menuItems[0][1]->Set(filterOn->value ? "active" : "bypassed", filterOn->value ? fOnIcon : fIcon, monoEngine->filter.filterOn->screenNode);
    menuState->menuItems[0][3]->Set("keytracked freqency", smallFIcon, (filterFreqKeyTrackParam->screenNode));
    menuState->menuItems[0][4]->Set("resonance", rIcon, filterResonanceParam->screenNode);
    menuState->menuItems[0][6]->Set("glide", glideIcon, glideParam->screenNode);
    menuState->menuItems[0][7]->Set("glide gate", glideIcon, glideGate->screenNode);
}

void MonoEngineClock::SetMenu(MenuState* menuState)
{
    menuState->menuItems[0][0]->Set("bpm", bpmIcon, bpmParam->screenNode);
    menuState->menuItems[0][1]->Set("division", divisionIcon, divisionParam->screenNode);
    menuState->menuItems[0][2]->Set(invert->value ? "inverted" : "not inverted", invert->value ? squareStartLowIcon : squareStartHighIcon, invert->screenNode);
    menuState->menuItems[0][3]->Set("reset gate", gateIcon, resetGate->screenNode);
}

void MonoEnginePhaser::SetMenu(MenuState* menuState)
{
    menuState->menuItems[0][0]->Set("source", sIcon, input->screenNode);
    menuState->menuItems[0][1]->Set(phaserOn->value ? "active" : "bypassed", phaserOn->value ? phOnIcon : phIcon, phaserOn->screenNode);
    menuState->menuItems[0][3]->Set("level", lIcon, levelParam->screenNode);
    menuState->menuItems[0][4]->Set("freqency", smallFIcon, frequencyParam->screenNode);
    menuState->menuItems[0][5]->Set("lfo frequency", fIcon, lfoFrequencyParam->screenNode);
    menuState->menuItems[0][6]->Set("lfo depth", depthIcon, lfoDepthParam->screenNode);
    menuState->menuItems[0][7]->Set("feedback", feedbackIcon, feedbackParam->screenNode);
}

void MonoEngineDelay::SetMenu(MenuState* menuState)
{
    menuState->menuItems[0][0]->Set("source", sIcon, input->screenNode);
    menuState->menuItems[0][1]->Set(delayOn->value ? "active" : "bypassed", delayOn->value ? delayOnIcon : delayIcon, delayOn->screenNode);
    menuState->menuItems[0][3]->Set("level", lIcon, levelParam->screenNode);
    menuState->menuItems[0][4]->Set("feedback", feedbackIcon, feedbackParam->screenNode);
    menuState->menuItems[0][5]->Set("time", timeIcon, delayTimeParam->screenNode);
}

void MonoEngineMixer::SetMenu(MenuState* menuState)
{
    menuState->menuItems[0][0]->Set("master level", lIcon,  masterLevel->screenNode);
    menuState->menuItems[1][0]->Set("source 1", sIcon, input1->screenNode);
    menuState->menuItems[1][1]->Set("level 1", lIcon,  input1Level->screenNode);
    menuState->menuItems[2][0]->Set("source 2", sIcon, input2->screenNode);
    menuState->menuItems[2][1]->Set("level 2", lIcon, input2Level->screenNode);
    menuState->menuItems[3][0]->Set("source 3", sIcon, input3->screenNode);
    menuState->menuItems[3][1]->Set("level 3", lIcon, input3Level->screenNode);
    menuState->menuItems[4][0]->Set("source 4", sIcon, input4->screenNode);
    menuState->menuItems[4][1]->Set("level 4", lIcon, input4Level->screenNode);
}

MonoEngineClock::MonoEngineClock() {
    screenNode = new ScreenNode(this);
    phase = 0;
}

MonoEngineOscillator::MonoEngineOscillator() {
    screenNode = new ScreenNode(this);
}

MonoEngineFilter::MonoEngineFilter() {
    screenNode = new ScreenNode(this);
}

MonoEngineDelay::MonoEngineDelay() {
    screenNode = new ScreenNode(this);
}

MonoEngineMixer::MonoEngineMixer() {
    screenNode = new ScreenNode(this);
}

MonoEnginePhaser::MonoEnginePhaser() {
    screenNode = new ScreenNode(this);
}

float getGlidedSemitone(float source, float destination, float percentage) {
    return source + (destination - source) * (1 - percentage);
}

void MonoEngine::AudioCallbackInit(size_t size)
{
    float bpm = clock.bpmParam->GetModulatedValue();
    for (int i = 0; i < 5; i++)
    {
        envelopes[i]->Update(bpm);
    }

    for (int i = 0; i < 2; i++) {
        lfos[i]->Update();
    }

    for (int i = 0; i < 3; i++) {
        if (!oscillators[i].glideGate->value) {
            oscillators[i].glideReset = true;
        }
    }

    if (filter.filterOn->value) {
        float glideTime = filter.glideParam->GetModulatedValue();
        if (glideTime) {
            filter.glideRemainingProportion -= size * ((1.0f / sampleRate) / glideTime);
            filter.glideRemainingProportion = filter.glideRemainingProportion < 0 ? 0 : filter.glideRemainingProportion;
        } else {
            filter.glideRemainingProportion = 0;
        }

        float f = getGlidedSemitone(filter.glideSourceSemitone, filter.glideDestinationSemitone, filter.glideRemainingProportion);
        f = filter.filterFreqKeyTrackParam->GetModulatedValue() * util::getFrequencyFromSemitone(f) * 8;

        filter.moogLadder->SetFreq(f > 40000 ? 40000 : f < 0 ? 0 : f);
        filter.moogLadder->SetRes(filter.filterResonanceParam->GetModulatedValue());
    }

    if (phaser.phaserOn->value) {
        float f = phaser.frequencyParam->GetModulatedValue();
        float lfoF = phaser.lfoFrequencyParam->GetModulatedValue();
        float depth = phaser.lfoDepthParam->GetModulatedValue();
        float fb = phaser.feedbackParam->GetModulatedValue();
        phaser.phaser->SetFreq(f > 40000 ? 40000 : f < 0 ? 0 : f);
        phaser.phaser->SetLfoFreq(lfoF > 40000 ? 40000 : lfoF < 0 ? 0 : lfoF);
        phaser.phaser->SetFeedback(fb > 1 ? 1 : fb < 0 ? 0 : fb);
        phaser.phaser->SetLfoDepth(depth > 1 ? 1 : depth < 0 ? 0 : depth);
    }

    if (delay.delayOn->value) {
        delay.delayLine->SetDelay(delay.delayTimeParam->GetModulatedValue() * (60.0f / bpm) * 48000.0f);
    }

    if (activeNotes.size())
    {
        MonoActiveNote activeNote = activeNotes.front();

        // avoid snapping directly to velocity
        if (velocityState < activeNote.velocity) {
            velocityState += 0.01;
            if (velocityState > activeNote.velocity) {
                velocityState = activeNote.velocity;
            }
        }
        if (velocityState > activeNote.velocity) {
            velocityState -= 0.01;
            if (velocityState < activeNote.velocity) {
                velocityState = activeNote.velocity;
            }
        }
        velocity->Set(velocityState);
    }
    if (activeNotes.size() || !useMidiNotes->value) {
        lastSemitone = semitone;
        semitone = util::getSemitoneFromMidiNote(useMidiNotes->value ? activeNotes.front().noteNumber : noteNumberParam->GetModulatedValue());
        if (lastSemitone != semitone || activeNotes.size() != lastActiveNotesCount)
        {
            for (int i = 0; i < 3; i++)
            {
                if (!oscillators[i].glideReset)
                {
                    oscillators[i].glideSourceSemitone = getGlidedSemitone(oscillators[i].glideSourceSemitone, oscillators[i].glideDestinationSemitone, oscillators[i].glideRemainingProportion);
                    oscillators[i].glideDestinationSemitone = semitone;
                    oscillators[i].glideRemainingProportion = 1;
                }
                else
                {
                    oscillators[i].glideReset = false;
                    oscillators[i].glideRemainingProportion = 0;
                    oscillators[i].glideSourceSemitone = semitone;
                    oscillators[i].glideDestinationSemitone = semitone;
                }
            }
            if (!filter.glideReset)
            {
                filter.glideSourceSemitone = getGlidedSemitone(filter.glideSourceSemitone, filter.glideDestinationSemitone, filter.glideRemainingProportion);
                filter.glideDestinationSemitone = semitone;
                filter.glideRemainingProportion = 1;
            }
            else
            {
                filter.glideReset = false;
                filter.glideRemainingProportion = 0;
                filter.glideSourceSemitone = semitone;
                filter.glideDestinationSemitone = semitone;
            }
        }
    }
    lastActiveNotesCount = activeNotes.size();
}

void MonoEngine::AudioCallbackPerSample()
{
    float bpm = clock.bpmParam->GetModulatedValue();
    ProcessClock(bpm);
    for (int i = 0; i < 5; i++)
    {
        envelopes[i]->Process();
    }
    for (int i = 0; i < 2; i++)
    {
        lfos[i]->Process(bpm);
    }
    ProcessOscillators();
    ProcessFilter();
    ProcessPhaser();
    ProcessDelay();
    ProcessMixers();
}

void MonoEngine::ProcessOscillators() {
    for (int i = 0; i < 3; i++)
    {
        float glideTime = oscillators[i].glideParam->GetModulatedValue();
        if (glideTime) {
            oscillators[i].glideRemainingProportion -= (1.0f / sampleRate) / glideTime;
            oscillators[i].glideRemainingProportion = oscillators[i].glideRemainingProportion < 0 ? 0 : oscillators[i].glideRemainingProportion;
        } else {
            oscillators[i].glideRemainingProportion = 0;
        }

        float glidedFreq = getGlidedSemitone(oscillators[i].glideSourceSemitone, oscillators[i].glideDestinationSemitone, oscillators[i].glideRemainingProportion);
        
        float f = util::getFrequencyFromSemitone(glidedFreq + oscillators[i].detuneSemitoneParam->GetModulatedValue());

        float sample = oscillators[i].levelParam->GetModulatedValue()
            * oscillators[i].waveFunction(oscillators[i].waveMorphParam->value, oscillators[i].phase + oscillators[i].phaseShiftParam->GetModulatedValue());

        oscillators[i].phase += f * (1.0 / sampleRate);
        if (oscillators[i].phase >= 1.0f)
        {
            oscillators[i].phase -= 1.0f;
        }

        oscillators[i].asModSource->Set(sample);
    }
}

void MonoEngine::ProcessClock(float bpm) {
    if (clock.resetGate->value) {
        if (!clock.inResetGate) {
            clock.phase = 0;
        }
        clock.inResetGate = true;
    } else {
        clock.inResetGate = false;
    }

    float f = util::bpmToHertz(bpm * clock.divisionParam->GetModulatedValue());
    clock.phase += f * (1.0 / sampleRate);
    if (clock.phase >= 1.0f)
    {
        clock.phase -= 1.0f;
    }
    clock.asModSource->Set(((clock.phase < 0.5) == clock.invert->value) ? 0 : 1);
}

void MonoEngine::ProcessFilter() {
    float sample = filter.input->GetMultipliedValue();

    if (filter.filterOn->value) {
        
        filter.asModSource->Set(filter.moogLadder->Process(sample));
    } else {
       filter.asModSource->Set(sample);
    }
}

void MonoEngine::ProcessPhaser() {
    float sample = phaser.input->GetMultipliedValue();
    if (phaser.phaserOn->value) {
        phaser.asModSource->Set(phaser.levelParam->GetModulatedValue() * phaser.phaser->Process(sample));
    } else {
        phaser.asModSource->Set(sample);
    }
}

void MonoEngine::ProcessDelay() {
    float sample = delay.input->GetMultipliedValue();
    if (delay.delayOn->value) {
        auto delayOut = delay.delayLine->Read();
        auto processedSample  = delayOut + sample;
        auto feedback = (delayOut * delay.feedbackParam->GetModulatedValue()) + sample;
        delay.delayLine->Write(feedback);
        delay.asModSource->Set(delay.levelParam->GetModulatedValue() * processedSample);
    } else {
        delay.asModSource->Set(sample);
    }
}

void MonoEngine::ProcessMixers() {
    for (int i = 0; i < 2; i++)
    {
        mixers[i].asModSource->Set(
            mixers[i].masterLevel->GetModulatedValue() * (
                mixers[i].input1->value * mixers[i].input1Level->GetModulatedValue() +
                mixers[i].input2->value * mixers[i].input2Level->GetModulatedValue() +
                mixers[i].input3->value * mixers[i].input3Level->GetModulatedValue() +
                mixers[i].input4->value * mixers[i].input4Level->GetModulatedValue())
        );
    }
}

void MonoEngine::HandleMidiMessage(MidiEvent m)
{
    if (m.type == NoteOn)
    {
        Noteon(util::getNoteNumber(m), util::getVelocity(m));
    }
    if (m.type == NoteOff)
    {
        Noteoff(util::getNoteNumber(m));
    }
    if (m.type == ChannelPressure) {
        channelPressure->Set(m.data[0] / 127.0f);
    }
    gate->Set(activeNotes.size() ? 1 : 0);
}

void MonoEngine::Init(float sampleRate)
{
    this->sampleRate = sampleRate;
    semitone = 60;

    clock.phase = 0;
    clock.inResetGate = false;

    for (int i = 0; i < 5; i++) {
        envelopes[i]->Init(sampleRate);
    }
    for (int i = 0; i < 2; i++) {
        lfos[i]->Init(sampleRate);
    }
    for (int i = 0; i < 3; i++)
    {
        oscillators[i].phase = 0;
        oscillators[i].glideSourceSemitone = 0;
        oscillators[i].glideDestinationSemitone = 0;
        oscillators[i].glideRemainingProportion = 0;
    }
    PreWire();
}

void MonoEngine::PreWire() {
    useMidiNotes->value = true;


    for (int i = 0; i < 5; i++) {
        gate->AssignDestination(envelopes[i]->gate);
    }

    // audio output 1: plain sine wave
    oscillators[0].waveMorphParam->value = 0;
    envelopes[0]->sustainParam->baseValue->value = 0.25;
    envelopes[0]->decayParam->baseValue->value = 0.2;
    gate->AssignDestination(oscillators[0].glideGate);
    envelopes[0]->modulationSource->AssignDestination(oscillators[0].levelParam->modulationDestinations[0]);
    velocity->AssignDestination(oscillators[0].levelParam->modulationDestinations[0]->multiplier);
    oscillators[0].asModSource->AssignDestination(globals.audioOutsModulationDestinations[0]);

    // audio output 2: triangle through phaser
    oscillators[1].waveMorphParam->value = 0.2;
    gate->AssignDestination(oscillators[1].glideGate);
    oscillators[1].glideParam->baseValue->value = 0.14;
    envelopes[1]->modulationSource->AssignDestination(oscillators[1].levelParam->modulationDestinations[0]);
    envelopes[1]->sustainParam->baseValue->value = 0.9;
    envelopes[1]->releaseParam->baseValue->value = 2;
    velocity->AssignDestination(oscillators[1].levelParam->modulationDestinations[0]->multiplier);
    oscillators[1].asModSource->AssignDestination(phaser.input);
    phaser.phaserOn->value = true;
    // avoid clipping
    phaser.levelParam->baseValue->value = 0.33;
    phaser.asModSource->AssignDestination(globals.audioOutsModulationDestinations[1]);
    channelPressure->AssignDestination(phaser.lfoFrequencyParam->modulationDestinations[0]);
    phaser.lfoFrequencyParam->modulationDestinations[0]->modulationSubtract->value = 0;
    phaser.lfoFrequencyParam->modulationDestinations[0]->modulationSubtract->value = 6;

    // audio output 3: square wave with envelope modulating filter instead of level
    oscillators[2].waveMorphParam->value = 1;
    oscillators[2].levelParam->baseValue->value = 1;
    oscillators[2].glideParam->baseValue->value = 0.07;
    gate->AssignDestination(oscillators[2].glideGate);
    envelopes[2]->modulationSource->AssignDestination(filter.filterFreqKeyTrackParam->modulationDestinations[0]);
    filter.filterFreqKeyTrackParam->modulationDestinations[0]->modulationSubtract->value = 0;
    filter.filterFreqKeyTrackParam->modulationDestinations[0]->modulationAdd->value = 2;
    velocity->AssignDestination(filter.filterFreqKeyTrackParam->modulationDestinations[0]->multiplier);
    oscillators[2].asModSource->AssignDestination(filter.input);
    filter.filterOn->value = true;
    filter.asModSource->AssignDestination(globals.audioOutsModulationDestinations[2]);
    channelPressure->AssignDestination(filter.filterResonanceParam->modulationDestinations[0]);
    filter.filterResonanceParam->modulationDestinations[0]->modulationAdd->value = 0.9;
    filter.filterResonanceParam->modulationDestinations[0]->modulationSubtract->value = 0;
    filter.filterResonanceParam->baseValue->value = 0;

    // audio output 4: first three outputs mixed together, with a delay added to the first signal, and an lfo fading a bit between the other two
    oscillators[0].asModSource->AssignDestination(delay.input);
    delay.levelParam->baseValue->value = 1;
    delay.delayOn->value = true;
    delay.delayTimeParam->baseValue->value = 1;
    delay.feedbackParam->baseValue->value = 0.66;
    channelPressure->AssignDestination(delay.feedbackParam->modulationDestinations[0]);
    delay.feedbackParam->modulationDestinations[0]->modulationSubtract->value = 0;
    delay.feedbackParam->modulationDestinations[0]->modulationAdd->value = 0.24;
    delay.asModSource->AssignDestination(mixers[0].input1);
    phaser.asModSource->AssignDestination(mixers[0].input2);
    filter.asModSource->AssignDestination(mixers[0].input3);
    mixers[0].input1Level->baseValue->value = 0.33;
    mixers[0].input2Level->baseValue->value = 0.33;
    mixers[0].input3Level->baseValue->value = 0.33;
    lfos[0]->modulationSource->AssignDestination(mixers[0].input2Level->modulationDestinations[0]);
    lfos[0]->modulationSource->AssignDestination(mixers[0].input3Level->modulationDestinations[0]);
    mixers[0].input2Level->modulationDestinations[0]->modulationAdd->value = 0.05;
    mixers[0].input2Level->modulationDestinations[0]->modulationSubtract->value = 0.05;
    mixers[0].input3Level->modulationDestinations[0]->modulationAdd->value = -0.05;
    mixers[0].input3Level->modulationDestinations[0]->modulationSubtract->value = -0.05;
    lfos[0]->periodParam->baseValue->value = 3;
    mixers[0].asModSource->AssignDestination(globals.audioOutsModulationDestinations[3]);

    // output divided clock to gate jack
    clock.asModSource->AssignDestination(globals.gateOutModulationDestination);
}

void MonoEngine::End()
{
}

MonoEngine::MonoEngine()
{
    #ifdef ENABLE_PRESET_DEVELOPMENT
    presets = new Presets("mono_engine");
    #endif

    for (int i = 0; i < MENU_WIDTH * 2; i++)
    {
        modulationSources.engineModulationSources[i] = 0;
    }

    noteNumberParam = new ComplexNumericParam(new FloatParam("note number", 60, 0, 127, 0.001, 1, 1), createNoteNumberModSub, createNoteNumberModAdd, "note number", noteNumberIcon);
    useMidiNotes = new BoolParam();

    clock.asModSource = new ModulationSource("clock out", timeIcon);
    clock.bpmParam = new ComplexNumericParam(new FloatParam("bpm", 110, 0, 666, 0.01, 1, 1), createBPMModSub, createBPMModAdd, "clock bpm", bpmIcon);
    clock.divisionParam = new ComplexNumericParam(new FloatParam("division", 4, 1, 32, 0.01, 1, 1), createDivisionModSub, createDivisionModAdd, "clock division", bpmIcon);
    clock.resetGate = new ModulationDestination("reset gate", gateIcon);
    clock.invert = new BoolParam();
    modulationSources.engineModulationSources[0] = clock.asModSource;

    for (int i = 0; i < 5; i++) {
        envelopes[i] = new Envelope("envelope " + std::to_string(i + 1), eIcons[i]);
        modulationSources.engineModulationSources[MENU_WIDTH + i] = envelopes[i]->modulationSource;
    }
    for (int i = 0; i < 2; i++) {
        lfos[i] = new LFO("lfo " + std::to_string(i + 1), lfoIcons[i]);
        modulationSources.engineModulationSources[MENU_WIDTH + 7 + i] = lfos[i]->modulationSource;
    }
    for (int i = 0; i < 3; i++)
    {
        oscillators[i].waveFunction = basicWaves;
        oscillators[i].glideGate = new ModulationDestination("ch" + std::to_string(i + 1) + " glide gate", glideIcon);

        oscillators[i].phaseShiftParam = new ComplexNumericParam(new PercentParam("phase shift", 0, 0, 1, 0.001, 0.1, 0.05), createZeroToHundredPercentModSub, createZeroToHundredPercentModAdd, "phase shift", phIcon);
        oscillators[i].glideParam = new ComplexNumericParam(new PercentParam("glide", 0, 0, 2, 0.001, 0.1, 0.01), createZeroToHundredPercentModSub, createZeroToHundredPercentModAdd, "ch" + std::to_string(i + 1) + " glide", glideIcon);
        oscillators[i].waveMorphParam = new WaveMorphParam(oscillators[i].waveFunction, "wave morph", 0, 0, 1, 0.001, 0.1, 0.22, oscillators[i].phaseShiftParam->baseValue);
        oscillators[i].detuneSemitoneParam = new ComplexNumericParam(new FloatParam("detune", 0, -88, 88, 0.01, 1, 1), createDetuneModSub, createDetuneModAdd, "ch" + std::to_string(i + 1) + " detune/fm", fmIcon);

        oscillators[i].levelParam = new ComplexNumericParam(new PercentParam("level", 0, 0, 1, 0.001, 0.05, 0.01), createZeroToHundredPercentModSub, createZeroToHundredPercentModAdd, "ch" + std::to_string(i + 1) + " level", lIcon);

        oscillators[i].asModSource = new ModulationSource("oscilator " + std::to_string(i + 1), i == 0 ? oscOut1Icon : i == 1 ? oscOut2Icon : i == 2 ? oscOut3Icon : oscOut4Icon);
        modulationSources.engineModulationSources[i + 2] = oscillators[i].asModSource;
    }

    filter.input = new ModulationDestination("filter input", fIcon);
    filter.glideParam = new ComplexNumericParam(new PercentParam("glide", 0, 0, 2, 0.001, 0.1, 0.01), createZeroToHundredPercentModSub, createZeroToHundredPercentModAdd, "filter glide", glideIcon);
    filter.glideGate = new ModulationDestination("filter glide gate", glideIcon);
    filter.moogLadder = new daisysp::MoogLadder();
    filter.moogLadder->Init(sampleRate);
    filter.filterOn = new BoolParam();
    filter.filterFreqKeyTrackParam = new ComplexNumericParam(new PercentParam("freq", 0, 0, 8, 0.001, 0.05, 0.01), createFreqModSub, createFreqModAdd, "keytracked freqency", fIcon);
    filter.filterResonanceParam = new ComplexNumericParam(new PercentParam("resonance", 0.5, 0, 1, 0.001, 0.05, 0.01), createZeroToHundredPercentModSub, createZeroToHundredPercentModAdd, "resonance", rIcon);
    filter.asModSource = new ModulationSource("filter out", fIcon);

    phaser.input = new ModulationDestination("phaser input", phIcon);
    phaser.phaser = new daisysp::Phaser();
    phaser.phaser->Init(sampleRate);
    phaser.phaserOn = new BoolParam();
    phaser.levelParam = new ComplexNumericParam(new PercentParam("level", 1, 0, 1, 0.001, 0.05, 0.01), createZeroToHundredPercentModSub, createZeroToHundredPercentModAdd, "phaser level", lIcon);
    phaser.asModSource = new ModulationSource("phaser out", phIcon);
    phaser.feedbackParam = new ComplexNumericParam(new PercentParam("feedback", 0.5, 0, 1, 0.001, 0.05, 0.01), createZeroToHundredPercentModSub, createZeroToHundredPercentModAdd, "phaser feedback", feedbackIcon);
    phaser.lfoDepthParam = new ComplexNumericParam(new PercentParam("lfo depth", 0.5, 0, 1, 0.001, 0.05, 0.01), createZeroToHundredPercentModSub, createZeroToHundredPercentModAdd, "phaser lfo depth", depthIcon);
    phaser.frequencyParam = new ComplexNumericParam(new FloatParam("freqency", 200, 0, 40000, 0.01, 50, 50), createRawFreqModSub, createRawFreqModAdd, "phaser freqency", smallFIcon);
    phaser.lfoFrequencyParam = new ComplexNumericParam(new FloatParam("lfo freqency", 1, 0, 40000, 0.01, 1, 1), createRawFreqModSub, createRawFreqModAdd, "phaser lfo freqency", fIcon);

    delay.input = new ModulationDestination("delay input", delayIcon);
    delay.delayLine = new daisysp::DelayLine<float, 36000>();
    delay.delayLine->Init();
    delay.delayOn = new BoolParam();
    delay.levelParam = new ComplexNumericParam(new PercentParam("level", 0, 0, 1, 0.001, 0.05, 0.01), createZeroToHundredPercentModSub, createZeroToHundredPercentModAdd, "delay level", lIcon);
    delay.feedbackParam = new ComplexNumericParam(new PercentParam("feedback", 0.5, 0, 1, 0.001, 0.05, 0.01), createZeroToHundredPercentModSub, createZeroToHundredPercentModAdd, "delay feedback", feedbackIcon);
    delay.delayTimeParam = new ComplexNumericParam(new PercentParam("time", 0.25, 0, 4, 0.001, 0.05, 0.01), createZeroToHundredPercentModSub, createZeroToHundredPercentModAdd, "delay time", feedbackIcon);
    delay.asModSource = new ModulationSource("delay out", delayIcon);

    for (int i = 0; i < 2; i++)
    {
        mixers[i].masterLevel = new ComplexNumericParam(new PercentParam("level", 1, 0, 1, 0.001, 0.05, 0.01), createZeroToHundredPercentModSub, createZeroToHundredPercentModAdd, "mixer " + std::to_string(i + 1) + " master level", lIcon);
        mixers[i].input1 = new ModulationDestination("mixer " + std::to_string(i + 1) + " input 1", mixIcon);
        mixers[i].input1Level = new ComplexNumericParam(new PercentParam("level", 0.25, 0, 1, 0.001, 0.05, 0.01), createZeroToHundredPercentModSub, createZeroToHundredPercentModAdd, "mixer " + std::to_string(i + 1) + " level 1", lIcon);
        mixers[i].input2 = new ModulationDestination("mixer " + std::to_string(i + 1) + " input 2", mixIcon);
        mixers[i].input2Level = new ComplexNumericParam(new PercentParam("level", 0.25, 0, 1, 0.001, 0.05, 0.01), createZeroToHundredPercentModSub, createZeroToHundredPercentModAdd, "mixer " + std::to_string(i + 1) + " level 2", lIcon);
        mixers[i].input3 = new ModulationDestination("mixer " + std::to_string(i + 1) + " input 3", mixIcon);
        mixers[i].input3Level = new ComplexNumericParam(new PercentParam("level", 0.25, 0, 1, 0.001, 0.05, 0.01), createZeroToHundredPercentModSub, createZeroToHundredPercentModAdd, "mixer " + std::to_string(i + 1) + " level 3", lIcon);
        mixers[i].input4 = new ModulationDestination("mixer " + std::to_string(i + 1) + " input 4", mixIcon);
        mixers[i].input4Level = new ComplexNumericParam(new PercentParam("level", 0.25, 0, 1, 0.001, 0.05, 0.01), createZeroToHundredPercentModSub, createZeroToHundredPercentModAdd, "mixer " + std::to_string(i + 1) + " level 4", lIcon);
        mixers[i].asModSource = new ModulationSource("mixer " + std::to_string(i + 1), i == 0 ? mix1Icon : mix2Icon);
        modulationSources.engineModulationSources[MENU_WIDTH + 10 + i] = mixers[i].asModSource;
    }

    gate = new ModulationSource("midi gate", gateIcon);
    velocity = new ModulationSource("velocity", velocityIcon);
    // default to 1 so the pre-wired envelope level multipliers have no effect when not using midi
    velocity->Set(1);
    channelPressure = new ModulationSource("channel pressure", pressureIcon);

    modulationSources.engineModulationSources[6] = filter.asModSource;
    modulationSources.engineModulationSources[7] = phaser.asModSource;
    modulationSources.engineModulationSources[8] = delay.asModSource;
    modulationSources.engineModulationSources[10] = gate;
    modulationSources.engineModulationSources[11] = velocity;
    modulationSources.engineModulationSources[12] = channelPressure;

    screenNode = new ScreenNode(this);
}

void MonoEngine::Noteon(int noteNumber, float velocity)
{
    activeNotes.push_front({noteNumber, velocity});
}

void MonoEngine::Noteoff(int noteNumber)
{
    for (std::list<MonoActiveNote>::iterator it = activeNotes.begin(); it != activeNotes.end(); ++it)
    {
        if (it->noteNumber == noteNumber)
        {
            it = activeNotes.erase(it);
        }
    }
}

void MonoEngine::SetMenu(MenuState* menuState)
{
    int y = 0;
    int x = 0;
    menuState->menuItems[y][x]->Set("clock", timeIcon, clock.screenNode);
    x += 2;
    menuState->menuItems[y][x]->Set(useMidiNotes->value ? "use midi notes" : "use note number param", useMidiNotes->value ? midiOnIcon : midiIcon, useMidiNotes->screenNode);
    if (!useMidiNotes->value) {
        x += 1;
        menuState->menuItems[y][x]->Set("note number", noteNumberIcon, noteNumberParam->screenNode);
    }
    y++;
    x = 0;
    menuState->menuItems[y][x]->Set("oscillator 1", o1Icon, oscillators[0].screenNode);
    x++;
    menuState->menuItems[y][x]->Set("oscillator 2", o2Icon, oscillators[1].screenNode);
    x++;
    menuState->menuItems[y][x]->Set("oscillator 3", o3Icon, oscillators[2].screenNode);
    x += 2;
    menuState->menuItems[y][x]->Set("envelope 1", eIcons[0], envelopes[0]->screenNode);
    x++;
    menuState->menuItems[y][x]->Set("envelope 2", eIcons[1], envelopes[1]->screenNode);
    x++;
    menuState->menuItems[y][x]->Set("envelope 3", eIcons[2], envelopes[2]->screenNode);
    x++;
    menuState->menuItems[y][x]->Set("envelope 4", eIcons[3], envelopes[3]->screenNode);
    x++;
    menuState->menuItems[y][x]->Set("envelope 5", eIcons[4], envelopes[4]->screenNode);
    x += 2;
    menuState->menuItems[y][x]->Set("lfo 1", lfoIcons[0], lfos[0]->screenNode);
    x++;
    menuState->menuItems[y][x]->Set("lfo 2", lfoIcons[1], lfos[1]->screenNode);
    y++;
    x = 0;
    menuState->menuItems[y][x]->Set("filter", fIcon, filter.screenNode);
    x++;
    menuState->menuItems[y][x]->Set("phaser", phIcon, phaser.screenNode);
    x++;
    menuState->menuItems[y][x]->Set("delay", delayIcon, delay.screenNode);
    x += 2;
    menuState->menuItems[y][x]->Set("mixer 1", mix1Icon, mixers[0].screenNode);
    x++;
    menuState->menuItems[y][x]->Set("mixer 2", mix2Icon, mixers[1].screenNode);
    y++;
    x = 0;
    menuState->menuItems[y][x]->Set("audio out 1", audioOut1Icon, globals.audioOutsModulationDestinations[0]->screenNode);
    x++;
    menuState->menuItems[y][x]->Set("audio out 2", audioOut2Icon, globals.audioOutsModulationDestinations[1]->screenNode);
    x++;
    menuState->menuItems[y][x]->Set("audio out 3", audioOut3Icon, globals.audioOutsModulationDestinations[2]->screenNode);
    x++;
    menuState->menuItems[y][x]->Set("audio out 4", audioOut4Icon, globals.audioOutsModulationDestinations[3]->screenNode);
    x += 2;
    menuState->menuItems[y][x]->Set("gate out", gateIcon, globals.gateOutModulationDestination->screenNode);
    x += 2;
    menuState->menuItems[y][x]->Set("cv out 1", cvOut1Icon, globals.cvOut1ModulationDestination->screenNode);
    x++;
    menuState->menuItems[y][x]->Set("cv out 2", cvOut2Icon, globals.cvOut2ModulationDestination->screenNode);
    y++;
    x = 0;
    menuState->menuItems[y][x]->Set("modulation sources", modIcon, modulationSources.screenNode);
    #ifdef ENABLE_PRESET_DEVELOPMENT
    x += 13;
    menuState->menuItems[y][x]->Set("presets", presetsIcon, presets->screenNode);
#endif
}


#endif
