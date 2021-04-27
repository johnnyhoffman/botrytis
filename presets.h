#ifndef PRESETS_H
#define PRESETS_H

#include <string>
#include "menu.h"
#include "screen_node.h"

class Presets : public IHasMenu
{
public:
    Presets(std::string rootPath);
    void SetMenu(MenuState* menuState);
    ScreenNode *screenNode = nullptr;

private:
    std::string rootPath;
};

#endif