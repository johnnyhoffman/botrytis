#ifndef WAVE_MORPH_PARAM_CPP
#define WAVE_MORPH_PARAM_CPP

#include "numeric_param.cpp"
#include <string>

class WaveMorphParam : public NumericParam
{
public:
    WaveFunction waveFunction;
    NumericParam *phaseShiftParam;
    std::string GetFormattedLabel()
    {
        int dec = std::abs((int)(GetValue() * 100000) % 100);
        return this->label
            + ": "
            + (std::to_string(
                (int)(GetValue() * 100)))
            + "."
            + (dec < 10 ? "0" : "")
            + (std::to_string(dec))
            + "%";
    }
    void RenderFill(daisy::DaisyPatch* patch)
    {
        int lastY = -1;
        for (int x = 0; x < SSD1309_WIDTH; x++)
        {
            int newY =
                SSD1309_HEIGHT
                    - 2
                    - (SSD1309_HEIGHT - 16) * (
                        (
                            waveFunction(
                                GetValue(),
                                (x + 0.5f)/SSD1309_WIDTH + (phaseShiftParam ? phaseShiftParam->value : 0)
                            )
                        + 1) / 2
                    );

            for (int y = lastY > 0 ? lastY : newY; ; y += (newY > lastY ? 1 : -1))
            {
                patch->display.DrawPixel(x, y, true);
                if (y == newY) break;
            }
            lastY = newY;
        }
    }
    WaveMorphParam(WaveFunction waveFunction, std::string label, float initialValue, float minValue, float maxValue, float minStep, float maxStep, float constStep, NumericParam* phaseShiftParam)
        : NumericParam(label, initialValue, minValue, maxValue, minStep, maxStep, constStep)
    {
        this->waveFunction = waveFunction;
        this->phaseShiftParam = phaseShiftParam;
    }
};

#endif