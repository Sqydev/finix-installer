#include "../../libs/esclib.h"

#include "../coredata.h"

char HandleCursor(int lowestPos, int maxPos, const int* skip, size_t skipSize) {
	if(IsKeyPressed(KEY_W)) {
		DrawChar(" ", 1, DATA.cursorPos, TERMWHITE);
		DATA.cursorPos--;

		for(size_t i = skipSize; i > 0; i--) {
			if(skip[i - 1] == DATA.cursorPos) { DATA.cursorPos--; }
		}

		if(DATA.cursorPos < lowestPos) {
			DATA.cursorPos = lowestPos;
			DrawChar(">", 1, DATA.cursorPos, TERMWHITE);
			return 1;
		}
	}
	if(IsKeyPressed(KEY_S)) {
		DrawChar(" ", 1, DATA.cursorPos, TERMWHITE);
		DATA.cursorPos++;

		for(size_t i = 0; i < skipSize; i++) {
			if(skip[i] == DATA.cursorPos) {
				DATA.cursorPos++;
			}
		}

		if(DATA.cursorPos > maxPos) {
			DATA.cursorPos = maxPos;
			DrawChar(">", 1, DATA.cursorPos, TERMWHITE);
			return -1;
		}
	}

	DrawChar(">", 1, DATA.cursorPos, TERMWHITE);

	return 0;
}

void SelectFromConstList(const char* title, const char* const* list, size_t sizeOfList, const char** dumpSelectedTo, bool* dumpIsSelected) {
	static bool inited = false;
	if(!inited) {
		DATA.cursorPos = 3;
		inited = true;
	}

	static int offset = 0;

	ClearTui(TERMBLACK, TERMWHITE);

	char currStat = HandleCursor(3, GetLastTuiIndex().y - 2, NULL, 0);
	if(currStat == 1) {
		if(offset > 0) { offset--; }
	}
	else if(currStat == -1) {
		if(offset < (int)sizeOfList) { offset++; }
	}

	DrawText(title, 0, 0, TERMWHITE);

	int i = 0;
	while(i < GetLastTuiIndex().y - 4 && i < (int)sizeOfList) {
		DrawText(list[i + offset], 4, i + 3, TERMWHITE);
		i++;
	}

	DrawText("-------------------------", 4, 2, TERMWHITE);
	DrawText("-------------------------", 4, i + 3, TERMWHITE);

	if(IsKeyPressed(KEY_ENTER)) {
		inited = false;

		*dumpSelectedTo = list[DATA.cursorPos + offset - 3];
		if(dumpIsSelected) { *dumpIsSelected = true; }
		DATA.screenState = SCREEN_MAIN;

		offset = 0;
	}
}
