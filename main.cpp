// TODO Failing to mount sd card. Remember to change to OPT ?= -Os instead of -O2 in libdaisy/core/Makefile and then "make clean; make" to make room in flash.
// #define ENABLE_PRESET_DEVELOPMENT

#include "daisy_patch.h"
#include "menu.cpp"
#include "globals.cpp"
#include "engine.cpp"
#include "modulation_source.cpp"
#include "modulation_sources.cpp"
#include "modulation_destination.cpp"
#include "wave_function.cpp"
#include "mono_engine.cpp"
#include "engine.cpp"
#include "engines.h"
#include "screen_node.cpp"
#include "numeric_param.cpp"
#include "percent_param.cpp"
#include "icons.cpp"
#include "daisysp.h"
#include <stdlib.h>
#include <string>
#include <list>
#include "envelope.cpp"
#include "fatfs.h"

using namespace daisy;

float sampleRate;

bool oledIsStale = true;
bool ignoreFallingEdge = false;
bool heldConsumed = false;

std::list<ScreenNode *> *screenNodeStack = nullptr;

ScreenNode *screenNode = nullptr;
MidiHandler midi;
MenuState* menuState = nullptr;

static int menuIndexToX(int index)
{
    return menuState->index % MENU_WIDTH;
}

static int menuIndexToY(int index)
{
    return menuState->index / MENU_WIDTH;
}

static void Enter(ScreenNode *toEnter)
{
    if (screenNode) {
        screenNodeStack->push_front(screenNode);
    }
    screenNode = toEnter;
    if (screenNode->engine && screenNode->engine != globals.engine)
    {
        if (globals.engine) {
            globals.engine->End();
        }
        globals.engine = screenNode->engine;
        globals.engine->Init(sampleRate);
    }
    if (screenNode->iHasMenu) {
        screenNode->iHasMenu->RefreshMenu(menuState, screenNode->savedMenuIndex);
    }
    if (screenNode->enter) {
        screenNode->enter(screenNode);
    }
}

static void Back()
{
    if (screenNode->back) {
        screenNode->back(screenNode);
    }
    if (!screenNodeStack->size()) return;
    screenNode = screenNodeStack->front();
    screenNodeStack->pop_front();
    if (screenNode->engine && screenNode->engine != globals.engine)
    {
        if (globals.engine) {
            globals.engine->End();
        }
        globals.engine = screenNode->engine;
        globals.engine->Init(sampleRate);
    }
    if (screenNode->iHasMenu) {
        screenNode->iHasMenu->RefreshMenu(menuState, screenNode->savedMenuIndex);
    }
    if (screenNode->modulationSource && !screenNode->modulationSource->destinations.size()) {
        Back();
    }
}

static void UpdateControls()
{
    globals.patch.ProcessDigitalControls();
    globals.patch.ProcessAnalogControls();
    int delta = globals.patch.encoder.Increment();
    bool fallingEdge = globals.patch.encoder.FallingEdge();
    float held = globals.patch.encoder.TimeHeldMs();
    if (fallingEdge && ignoreFallingEdge) {
        ignoreFallingEdge = false;
    }
    else if (fallingEdge && (screenNode->type == MenuScreenNode || screenNode->type == ModulationSourceScreenNode) && menuState->index >= 0)
    {
        MenuItem* menuItem = menuState->menuItems[menuIndexToY(menuState->index)][menuIndexToX(menuState->index)];
        if (menuItem->screenNode)
        {
            if (menuItem->screenNode->type == MenuScreenNode || (menuItem->screenNode->type == ModulationSourceScreenNode && !globals.modulationDestinationForMenu))
            {
                Enter(menuItem->screenNode);
            }
            else if (menuItem->screenNode->type == NumericParamScreenNode)
            {
                Enter(menuItem->screenNode);
            }
            else if (menuItem->screenNode->type == BoolParamScreenNode)
            {
                menuItem->screenNode->boolParam->value = !menuItem->screenNode->boolParam->value;
                screenNode->iHasMenu->RefreshMenu(menuState, screenNode->savedMenuIndex);
            }
            else if (menuItem->screenNode->type == ModulationSourceScreenNode)
            {
                if (menuItem->screenNode->modulationSource) {
                    globals.modulationDestinationForMenu->Clear();
                    menuItem->screenNode->modulationSource->AssignDestination(globals.modulationDestinationForMenu);
                    screenNode->iHasMenu->RefreshMenu(menuState, screenNode->savedMenuIndex);
                }
                else {
                    globals.modulationDestinationForMenu->Clear();
                    Back();
                }
            }
        }
        oledIsStale = true;
    }
    else if (fallingEdge && screenNode->type == NumericParamScreenNode)
    {
        screenNode->numericParam->ToggleMode();
    }
    if (held > 500)
    {
        if (!heldConsumed) {
            Back();
            ignoreFallingEdge = true;
            heldConsumed = true;
            oledIsStale = true;
        }
    } else {
        heldConsumed = false;
    }

    if (delta && (screenNode->type == MenuScreenNode || screenNode->type == ModulationSourceScreenNode))
    {
        while ((delta > 0 || menuState->index > -1) && (delta < 0 || menuState->index < (MENU_WIDTH * MENU_HEIGHT) - 1))
        {
            menuState->index += delta;
            screenNode->savedMenuIndex = menuState->index;
            if (menuState->index == -1 || menuState->menuItems[menuIndexToY(menuState->index)][menuIndexToX(menuState->index)]->screenNode)
                break;
        }
        oledIsStale = true;
    }
    else if (delta && screenNode->type == NumericParamScreenNode)
    {
        screenNode->numericParam->Delta(delta, globals.patch.seed.system.GetNow());
        oledIsStale = true;
    }

    for (int i = 0; i < 4; i++)
    {
        modulationSources.ctrls[i]->Set(globals.patch.controls[i].Process());
    }
    for (int i = 0; i < 2; i++) {
        modulationSources.gateIns[i]->Set(globals.patch.gate_input[i].State());
    }
}

static void RenderLabel(const char *label)
{
    globals.patch.display.SetCursor(0, 0);
    globals.patch.display.WriteString(label, Font_6x8, true);
}

static MenuItem* GetCurrentMenuIcon()
{
    return menuState->menuItems[menuIndexToY(menuState->index)][menuIndexToX(menuState->index)];
}

static void RenderMenuIcon(int x, int y, MenuItem* menuItem, bool isSelected)
{
    if (!menuItem->screenNode) return;

    int pixBaseY = 9 + (y * 9);
    int pixBaseX = x * 9;

    if (isSelected)
    {
        for (int x = 0; x < 10; x++)
        {
            for (int y = 0; y < 10; y++)
            {
                if (x == 0 || y == 0 || x == 9 || y == 9)
                {
                    globals.patch.display.DrawPixel(pixBaseX + x, pixBaseY + y, true);
                }
            }
        }
    }

    pixBaseY++;
    pixBaseX++;

    for (int x = 0; x < 8; x++)
    {
        for (int y = 0; y < 8; y++)
        {
            if (menuItem->icon->icon[y][x])
            {
                globals.patch.display.DrawPixel(pixBaseX + x, pixBaseY + y, true);
            }
        }
    }
}

static void UpdateOled()
{
    if (oledIsStale)
    {
        oledIsStale = false;
        globals.patch.display.Fill(false);
        if (screenNode->type == MenuScreenNode || screenNode->type == ModulationSourceScreenNode)
        {
            if (menuState->index >= 0)
            {
                MenuItem* menuItem = GetCurrentMenuIcon();
                if (menuItem->screenNode)
                {
                    RenderLabel(menuItem->label.c_str());
                }
            }
            for (int x = 0; x < MENU_WIDTH; x++)
            {
                for (int y = 0; y < MENU_HEIGHT; y++)
                {
                    RenderMenuIcon(x, y, menuState->menuItems[y][x], menuState->index >= 0 && y == menuIndexToY(menuState->index) && x == menuIndexToX(menuState->index));
                }
            }
        }
        else if (screenNode->type == NumericParamScreenNode)
        {
            RenderLabel(screenNode->numericParam->GetFormattedLabel().c_str());
            screenNode->numericParam->RenderFill(&globals.patch);
        }
        globals.patch.display.Update();
    }
}

static void AudioCallback(float **in, float **out, size_t size)
{
    midi.Listen();
    while (midi.HasEvents())
    {
        MidiEvent event = midi.PopEvent();
        if (globals.engine)
        {
            globals.engine->HandleMidiMessage(event);
        }
    }
    UpdateControls();
    UpdateOled();

    if (globals.engine)
    {
        globals.engine->AudioCallback(in, out, size);
    }
    else
    {
        for (unsigned int n = 0; n < size; n++)
        {
            for (size_t chn = 0; chn < 4; chn++)
            {
                out[chn][n] = 0;
            }
        }
    }
}

void SetGlobalModulationDestinations() {
    globals.gateOutModulationDestination = new ModulationDestination("gate out", gateIcon);
    globals.cvOut1ModulationDestination = new ModulationDestination("cv out 1", cvOut1Icon);
    globals.cvOut2ModulationDestination = new ModulationDestination("cv out 2", cvOut2Icon);
    for (int i = 0; i < 4; i++) {
        globals.audioOutsModulationDestinations[i] = new ModulationDestination(
            "audio out " + std::to_string(i + 1),
            i == 0 ? audioOut1Icon : i == 1 ? audioOut2Icon : i == 2 ? audioOut3Icon : audioOut4Icon);
    }
}

int main(void)
{
    menuState = new MenuState();
    screenNodeStack = new std::list<ScreenNode *>();
    globals.patch.Init();
    globals.emptyModSourceScreenNode = new ScreenNode((ModulationSource *)nullptr, ModulationSourceScreenNode);
    sampleRate = globals.patch.AudioSampleRate();
    // TODO set MidiHandler::OUTPUT_MODE_UART1 and send midi notes through
    midi.Init(MidiHandler::INPUT_MODE_UART1, MidiHandler::OUTPUT_MODE_NONE);
    midi.StartReceive();
    SetGlobalModulationSources();
    SetGlobalModulationDestinations();
    monoEngine = new MonoEngine();

    globals.patch.StartAdc();
    globals.patch.StartAudio(AudioCallback);
    Enter(monoEngine->screenNode);
}