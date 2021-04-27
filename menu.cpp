#ifndef MENU_CPP
#define MENU_CPP

#include <string>
#include "menu.h"
#include "screen_node.cpp"

class MenuItem
{
public:
    std::string label;
    Icon* icon;
    ScreenNode *screenNode = nullptr;

    void Set(std::string label, Icon* icon, ScreenNode *screenNode) {
        this->label = label;
        this->icon = icon;
        this->screenNode = screenNode;
    }
};

void IHasMenu::RefreshMenu(MenuState* menuState, int index) {
    menuState->index = index;
    for (int x = 0; x < MENU_WIDTH; x++)
    {
        for (int y = 0; y < MENU_HEIGHT; y++)
        {
            menuState->menuItems[y][x]->screenNode = nullptr;
        }
    }
    SetMenu(menuState);
};

MenuState::MenuState() {
    for (int x = 0; x < MENU_WIDTH; x++)
    {
        for (int y = 0; y < MENU_HEIGHT; y++)
        {
            menuItems[y][x] = new MenuItem();
        }
    } 
}

#endif