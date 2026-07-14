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
