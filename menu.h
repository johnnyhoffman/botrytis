#ifndef MENU_H
#define MENU_H

const int MENU_WIDTH = 14;
const int MENU_HEIGHT = 6;

class MenuItem;

class MenuState
{
public:
    int index = -1;
    MenuItem* (menuItems)[MENU_HEIGHT][MENU_WIDTH];
    MenuState();
};

class IHasMenu {
public:
    virtual void SetMenu(MenuState* menuState) = 0;
    void RefreshMenu(MenuState *menuState, int index);
};

#endif