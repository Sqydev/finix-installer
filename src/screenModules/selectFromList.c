#include "../../libs/esclib.h"

#include "./screenModules.h"
#include "../coredata.h"

#include "../../libs/esclib.h"

#include "./screenModules.h"
#include "../coredata.h"

void SelectFromConstList(const char* title, const char* const* list, size_t sizeOfList, const char** dumpSelectedTo, bool* dumpIsSelected) {
    static bool inited = false;
    static int offset = 0;

    if(!inited) {
        DATA.cursorPos = 3;
        offset = 0;
        inited = true;
    }

    ClearTui(TERMBLACK, TERMWHITE);

    int visible = GetLastTuiIndex().y - 4;
    if(visible < 1) { visible = 1; }

    int shown = (int)sizeOfList;
    if(shown > visible) { shown = visible; }

    int maxOffset = (int)sizeOfList - visible;
    if(maxOffset < 0) { maxOffset = 0; }

    int maxCursorPos = 3 + shown - 1;

    char currStat = HandleCursor(3, maxCursorPos, NULL, 0);

    if(currStat == 1) { if(offset > 0) { offset--; } }
    else if(currStat == -1) { if(offset < maxOffset) { offset++; } }

    DrawText(title, 0, 0, TERMWHITE);

    for(int i = 0; i < shown; i++) {
        DrawText(list[offset + i], 4, 3 + i, TERMWHITE);
    }

    DrawText("-------------------------", 4, 2, TERMWHITE);
    DrawText("-------------------------", 4, 3 + shown, TERMWHITE);

    if(IsKeyPressed(KEY_ENTER)) {
        int selected = offset + (DATA.cursorPos - 3);

        if(selected >= 0 && selected < (int)sizeOfList) {
            *dumpSelectedTo = list[selected];

            if(dumpIsSelected) { *dumpIsSelected = true; }
        }

        offset = 0;
        inited = false;
        DATA.screenState = SCREEN_MAIN;
    }
}

void SelectFromConstListORCustom(const char* title, const char* const* list, size_t sizeOfList, const char** dumpSelectedTo, bool* dumpIsSelected) {
    static bool inited = false;
    static int offset = 0;

    if(!inited) {
        DATA.cursorPos = 3;
        offset = 0;
        inited = true;
    }

    ClearTui(TERMBLACK, TERMWHITE);

    int visible = GetLastTuiIndex().y - 4;
    if(visible < 1) { visible = 1; }

    int shown = (int)sizeOfList;
    if(shown > visible) { shown = visible; }

    int maxOffset = (int)sizeOfList - visible;
    if(maxOffset < 0) { maxOffset = 0; }

    int maxCursorPos = 3 + shown - 1 - 1;

    char currStat = HandleCursor(3, maxCursorPos, NULL, 0);

    if(currStat == 1) { if(offset > 0) { offset--; } }
    else if(currStat == -1) { if(offset < maxOffset) { offset++; } }

    DrawText(title, 0, 0, TERMWHITE);

    for(int i = 0; i < shown; i++) {
        DrawText(list[offset + i], 4, 3 + i, TERMWHITE);
    }

    DrawText("-------------------------", 4, 2, TERMWHITE);
    DrawText("-------------------------", 4, 3 + shown, TERMWHITE);

    if(IsKeyPressed(KEY_ENTER)) {
        int selected = offset + (DATA.cursorPos - 3);

        if(selected >= 0 && selected < (int)sizeOfList) {
            *dumpSelectedTo = list[selected];

            if(dumpIsSelected) { *dumpIsSelected = true; }
        }

        offset = 0;
        inited = false;
        DATA.screenState = SCREEN_MAIN;
    }
}
