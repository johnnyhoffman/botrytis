#ifndef LINEAR_FILL_NUMERIC_PARAM_CPP
#define LINEAR_FILL_NUMERIC_PARAM_CPP

#include "numeric_param.cpp"
#include "daisy_patch.h"
#include <string>

int availableFillArea = SSD1309_WIDTH * (SSD1309_HEIGHT - 10);
class LinearFillNumericParam : public NumericParam
{
public:
    using NumericParam::NumericParam;
    void RenderFill(daisy::DaisyPatch* patch)
    {
        float zeroToOne = GetValue() <= minValue  ? minValue : GetValue() >= maxValue ? maxValue : ((GetValue() - minValue) / (maxValue - minValue));
            
        int pixels = (int)round(zeroToOne * availableFillArea);
        int filledPixels = 0;
        for (int y = 0; y < SSD1309_HEIGHT; y++)
        {
            for (int x = 0; x < SSD1309_WIDTH; x++)
            {
                if (filledPixels >= pixels)
                    return;
                patch->display.DrawPixel(x, y + 10, true);
                filledPixels++;
            }
        }
    }
};

#endif