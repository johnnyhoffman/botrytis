#ifndef NUMERIC_PARAM_CPP
#define NUMERIC_PARAM_CPP

#include "numeric_param.h"
#include "util.cpp"
#include "modulation_source.cpp"
#include <string>
#include "daisy_patch.h"
#include "screen_node.cpp"

class NumericParam
{
public:
    float minValue;
    float maxValue;
    float value;
    ScreenNode *screenNode;
    void ToggleMode()
    {
        isScalingDeltaMode = !isScalingDeltaMode;
    }
    void Delta(int delta, u_int32_t ticks)
    {
        if (isScalingDeltaMode) {
            ScalingDelta(delta, ticks);   
        }
        else
        {
            ConstDelta(delta);
        }
    }

    float GetValue()
    {
        return value;
    }
    virtual std::string GetFormattedLabel() = 0;
    virtual void RenderFill(daisy::DaisyPatch* patch) = 0;
    NumericParam(std::string label, float initialValue, float minValue, float maxValue, float minStep, float maxStep, float constStep)
    {
        this->label = label;
        this->value = initialValue;
        this->value = initialValue;
        this->minValue = minValue;
        this->maxValue = maxValue;
        this->minStep = minStep;
        this->maxStep = maxStep;
        this->step = minStep;
        this->constStep = constStep;
        this->screenNode = new ScreenNode(this);
    }
    bool ConsumeDirty() {
        bool d = dirty;
        dirty = false;
        return d;
    }

private:
    bool dirty;
    float step;
    bool isScalingDeltaMode = true;
    u_int32_t lastTick;
    float constStep;
    float minStep;
    float maxStep;

    void ScalingDelta(int delta, u_int32_t ticks)
    {
        u_int32_t ticksElapsed = ticks - lastTick;
        float stepStep = (maxStep - minStep) / 1000;
        value += delta * step;
        value = value > maxValue ? maxValue : value < minValue ? minValue : value;
        // TODO reset step if direction has changed
        if (ticksElapsed < 200)
        {
            step += (200.0f / (float)ticksElapsed) * stepStep;
        }
        else
        {
            step -= pow((float)ticksElapsed / 200.0f, 4) * stepStep;
        }
        step = step > maxStep ? maxStep : step < minStep ? minStep : step;
        dirty = true;
        lastTick = ticks;
    }
    void ConstDelta(int delta)
    {
        float modded = util::floatMod(value, constStep, true);
        if (modded >= 0.000001)
        {
            if (value > 0)
            {
                if (delta > 0)
                {
                    value += (constStep - modded);
                }
                else
                {
                    value -= modded;
                }
            }
            else
            {
                if (delta > 0)
                {
                    value += modded;
                }
                else
                {
                    value -= (constStep - modded);
                }
            }
        }

        value += delta * constStep;

        value = value > maxValue ? maxValue : value < minValue ? minValue : value;
    }

protected:
    std::string label;
};

#endif