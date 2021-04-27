#ifndef FLOAT_PARAM_CPP
#define FLOAT_PARAM_CPP

#include "linear_fill_numeric_param.cpp"
#include <string>

class FloatParam : public LinearFillNumericParam
{
public:
    using LinearFillNumericParam::LinearFillNumericParam;
    std::string GetFormattedLabel()
    {
        int dec = std::abs((int)((GetValue() - (int)GetValue()) * 100));
        return this->label + ": "
            + (std::to_string(
                (int)GetValue()))
            + "."
            + (dec < 10 ? "0" : "")
            + std::to_string(dec);
    }
};

#endif