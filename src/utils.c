#include "../libs/esclib.h"

#include "./coredata.h"
#include "fns.h"

#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>

void HandleCursor(int lowestPos, int maxPos, const int* skip, size_t skipSize) {
	if(IsKeyPressed(KEY_W)) {
		DATA.cursorPos--;
		if(DATA.cursorPos < lowestPos) { DATA.cursorPos = lowestPos; }

		for(size_t i = skipSize; i > 0; i--) {
			if(skip[i - 1] == DATA.cursorPos) { DATA.cursorPos--; }
		}
	}
	if(IsKeyPressed(KEY_S)) {
		DATA.cursorPos++;
		if(DATA.cursorPos > maxPos) { DATA.cursorPos = maxPos; }
		
		for(size_t i = 0; i < skipSize; i++) {
			if(skip[i] == DATA.cursorPos) { DATA.cursorPos++; }
		}
	}

	DrawChar(">", 1, DATA.cursorPos, TERMWHITE);
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

void SelectFromList(const char* const* list, size_t size, size_t* dumpSelectedTo, bool* dumpIsSelected) {
	HandleCursor(0, GetLastTuiIndex().y, NULL, 0);

	for(int i = 0; i < GetLastTuiIndex().x && i < (int)size; i++) {
		DrawText(list[i], 4, i, TERMWHITE);
	}

	if(IsKeyPressed(KEY_ENTER)) {
		
	}
}
