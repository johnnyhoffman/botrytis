#ifndef PERCENT_PARAM_CPP
#define PERCENT_PARAM_CPP

#include "linear_fill_numeric_param.cpp"
#include <string>

class PercentParam : public LinearFillNumericParam
{
public:
    using LinearFillNumericParam::LinearFillNumericParam;
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
};

#endif