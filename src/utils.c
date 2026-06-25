#include "../libs/esclib.h"

#include "./coredata.h"
#include "fns.h"

#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>

char HandleCursor(int lowestPos, int maxPos, const int* skip, size_t skipSize) {
	if(IsKeyPressed(KEY_W)) {
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

char** ExtractFile(char* path, size_t* dumpLinesCount) {
	(void)path;
	(void)dumpLinesCount;
	return NULL;
}

char** ExtractDirContents(char* path, size_t* dumpContentCount) {
	DIR* dir = opendir(path);
	
	struct dirent* entry;

	char** contents = NULL;
	size_t contentsCount = 0;

	while((entry = readdir(dir)) != NULL) {
		contentsCount++;
	}

	contents = malloc(contentsCount * sizeof(char*));

	rewinddir(dir);

	for(size_t i = 0; (entry = readdir(dir)) != NULL; i++) {
		contents[i] = strdup(entry->d_name);
	}

	closedir(dir);

	*dumpContentCount = contentsCount;
	return contents;
}

void SelectFromList(const char* title, const char* const* list, size_t size, size_t* dumpSelectedTo, bool* dumpIsSelected) {
	static bool inited = false;
	if(!inited) { DATA.cursorPos = 3; inited = true; }

	static int offset = 0;

	char currStat = HandleCursor(3, GetLastTuiIndex().y - 2, NULL, 0);
	if(currStat == 1) {
		if(offset > 0) { offset--; }
	}
	else if(currStat == -1) {
		if(offset < (int)size) { offset++; }
	}

	DrawText(title, 0, 0, TERMWHITE);

	int i = 0;
	while(i < GetLastTuiIndex().y - 4 && i < (int)size) {
		DrawText(list[i + offset], 4, i + 3, TERMWHITE);
		i++;
	}

	DrawText("-------------------------", 4, 2, TERMWHITE);
	DrawText("-------------------------", 4, i + 3, TERMWHITE);

	if(IsKeyPressed(KEY_ENTER)) {
		inited = false;

		*dumpSelectedTo = DATA.cursorPos + offset - 3;
		if(dumpIsSelected) { *dumpIsSelected = true; }
		DATA.currScreen = SCREEN_MAIN;

		offset = 0;
	}
}
