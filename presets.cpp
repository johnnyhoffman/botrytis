#ifndef PRESETS_CPP
#define PRESETS_CPP

#include "presets.h"
#include <string>
#include "menu.cpp"
#include "icons.cpp"
#include "screen_node.cpp"
#include "bool_param.cpp"
#include "daisy_patch.h"
#include "daisysp.h"
#include "fatfs.h"


Presets::Presets(std::string rootPath) {
    this->rootPath = rootPath;
    screenNode = new ScreenNode(this);
}

void Presets::SetMenu(MenuState* menuState) {
    daisy::SdmmcHandler sd;
    char outbuff[512];
    char inbuff[512];
    size_t len, byteswritten;
    sprintf(outbuff, "Daisy...Testing...\n1...\n2...\n3...\n");
    memset(inbuff, 0, 512);
    len = strlen(outbuff);


    // Init SD Card
    daisy::SdmmcHandler::Config sd_cfg;
    sd_cfg.Defaults();
    sd.Init(sd_cfg);

    // Links libdaisy i/o to fatfs driver.
    dsy_fatfs_init();

    // TODO FAILS HERE
    // f_mount(&SDFatFS, SDPath, 1);

    // if(f_open(&SDFile, "test_file", (FA_CREATE_ALWAYS) | (FA_WRITE))
    //    == FR_OK)
    // {
    //     f_write(&SDFile, outbuff, len, &byteswritten);
    //     f_close(&SDFile);
    // }

    // if(f_open(&SDFile,  "test_file", FA_READ) == FR_OK)
    // {
    //     f_read(&SDFile, inbuff, len, &byteswritten);
    //     f_close(&SDFile);
    // }

    // auto s = std::string(inbuff);

    // for (int x = 0; x < MENU_WIDTH; x++)
    // {
    //     for (int y = 0; y < MENU_HEIGHT; y++)
    //     {
    //         menuState->menuItems[y][x]->Set(s, xIcon, (new BoolParam())->screenNode);
    //     }
    // }
}

#endif