#ifndef BOOL_PARAM_CPP
#define BOOL_PARAM_CPP

#include "bool_param.h"

class BoolParam
{
public:
    bool value;
    ScreenNode *screenNode = nullptr;
    void Toggle()
    {
        value =! value;
    }
    BoolParam() {
        screenNode = new ScreenNode(this);
        value = false;
    }
};

#endif