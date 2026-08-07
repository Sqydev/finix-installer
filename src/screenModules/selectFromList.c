#include "../../libs/esclib.h"

#include "./screenModules.h"
#include "../coredata.h"

#include "../../libs/esclib.h"

#include "./screenModules.h"
#include "../coredata.h"
#include <stdlib.h>
#include <string.h>

void SelectFromListOnly(const char* title, const char* const* list, size_t sizeOfList, char** dumpSelectedTo, bool* dumpIsSelected) {
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
			*dumpSelectedTo = (char*)list[selected];

			if(dumpIsSelected) { *dumpIsSelected = true; }
		}

		offset = 0;
		inited = false;
		DATA.screenState = SCREEN_MAIN;
	}
}

void SelectFromList(const char* title, const char* customPrompt, const char* const* list, size_t sizeOfList, char** dumpSelectedTo, bool* dumpIsSelected, size_t* dumpSizeOfTo, bool* dumpIsCustomTo) {
	static bool inited = false;
	static int offset = 0;

	const int totalItems = (int)sizeOfList + 1;

	if(!inited) {
		DATA.cursorPos = 3;
		offset = 0;
		inited = true;
	}

	ClearTui(TERMBLACK, TERMWHITE);

	int visible = GetLastTuiIndex().y - 4;
	if(visible < 1) { visible = 1; }

	int shown = totalItems;
	if(shown > visible) { shown = visible; }

	int maxOffset = totalItems - visible;
	if(maxOffset < 0) { maxOffset = 0; }

	int maxCursorPos = 3 + shown - 1;

	char currStat = HandleCursor(3, maxCursorPos, NULL, 0);

	if(currStat == 1) {
		if(offset > 0) { offset--; }
	}
	else if(currStat == -1) {
		if(offset < maxOffset) { offset++; }
	}

	DrawText(title, 0, 0, TERMWHITE);

	for(int i = 0; i < shown; i++) {
		int index = offset + i;

		if(index == 0) {
			DrawTextf("%s", 4, 3 + i, TERMWHITE, customPrompt);
		}
		else {
			DrawText(list[index - 1], 4, 3 + i, TERMWHITE);
		}
	}

	DrawText("-------------------------", 4, 2, TERMWHITE);
	DrawText("-------------------------", 4, 3 + shown, TERMWHITE);

	if(IsKeyPressed(KEY_ENTER)) {
		int selected = offset + (DATA.cursorPos - 3);

		if(selected == 0) {
			int currentCursorRow = DATA.cursorPos;
			char* tring = GetStringFromUser((Vector2i){ 4 + strlen(customPrompt), currentCursorRow });
		
			if(!tring) {
				DATA.screenState = SCREEN_MAIN;
				return;
			}

			if(*dumpIsCustomTo && *dumpSelectedTo) { 
				free(*dumpSelectedTo); 
				*dumpSelectedTo = NULL;
			}

			if(tring[0] == '\0') {
				offset = 0;
				inited = false;
				DATA.screenState = SCREEN_MAIN;

				free(tring);

				*dumpIsCustomTo = false;
				*dumpIsSelected = false;
				*dumpSizeOfTo = 0;
				*dumpSelectedTo = NULL;

				return;
			}


			*dumpSelectedTo = tring;
			*dumpIsCustomTo = true;
		}
		else {
			if(*dumpIsCustomTo && *dumpSelectedTo && *dumpIsSelected) { free(*dumpSelectedTo); }
			*dumpSelectedTo = (char*)list[selected - 1];
			*dumpIsCustomTo = false;
		}

		*dumpSizeOfTo = strlen(*dumpSelectedTo);
		*dumpIsSelected = true;

		offset = 0;
		inited = false;
		DATA.screenState = SCREEN_MAIN;
	}
}
