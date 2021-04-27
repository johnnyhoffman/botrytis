#ifndef WAVE_FUNCTION_CPP
#define WAVE_FUNCTION_CPP

#include <string>
#include "util.cpp"

typedef float (*WaveFunction)(float position, float phase);

float basicWaves(float position, float phase) {
    phase = phase - (int)phase;
    phase = phase < 0 ? phase + 1 : phase;
    if (position < 0.125) return util::sine(phase);
    if (position < 0.25) return util::triangle(phase);
    if (position < 0.375) return util::sawUp(phase);
    if (position < 0.5) return util::sawDown(phase);
    if (position < 0.625) return util::step(phase);
    if (position < 0.75) return util::doubleStep(phase);
    // if (position < 0.875) return ...
    return util::square(phase);
}

float lfoWaves(float position, float phase) {
    phase = phase - (int)phase;
    phase = phase < 0 ? phase + 1 : phase;
    if (position < 0.2) return util::sine(phase + 0.25);
    if (position < 0.4) return util::triangle(phase / 2) * 2 - 1;
    if (position < 0.6) return util::sawUp(phase / 2) * 2 - 1;
    if (position < 0.8) return util::sawDown(phase / 2) * 2 + 1;
    return util::square(phase);
}

#endif