#ifndef SCREEN_NODE_CPP
#define SCREEN_NODE_CPP

#include "screen_node.h"
#include "menu.h"
#include "numeric_param.h"
#include "bool_param.h"
#include "complex_numeric_param.h"
#include "modulation_source.h"
#include "modulation_destination.h"
#include "engine.h"
#include "util.cpp"

typedef void (*ScreenNodeAction)(ScreenNode*);
class ScreenNode
{
public:
    ScreenNodeType type;
    IHasMenu *iHasMenu = nullptr;
    Engine *engine = nullptr;
    NumericParam *numericParam = nullptr;
    BoolParam *boolParam = nullptr;
    ModulationSource *modulationSource = nullptr;
    ScreenNodeAction enter = nullptr;
    ScreenNodeAction back = nullptr;
    int savedMenuIndex = -1;

    ScreenNode(ScreenNodeType type)
    {
        this->type = type;
    }
    ScreenNode(IHasMenu *menu)
    {
        this->type = MenuScreenNode;
        this->iHasMenu = menu;
        this->modulationSource = dynamic_cast<ModulationSource*>(menu);
        this->engine = dynamic_cast<Engine*>(menu);
    }
    ScreenNode(IHasMenu *menu, ScreenNodeType type)
    {
        this->type = type;
        this->iHasMenu = menu;
        this->modulationSource = dynamic_cast<ModulationSource*>(menu);
        this->engine = dynamic_cast<Engine*>(menu);
    }
    ScreenNode(NumericParam *numericParam)
    {
        this->type = NumericParamScreenNode;
        this->numericParam = numericParam;
    }
    ScreenNode(BoolParam *boolParam)
    {
        this->type = BoolParamScreenNode;
        this->boolParam = boolParam;
    }
};

#endif