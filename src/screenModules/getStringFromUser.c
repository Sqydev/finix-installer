#include "../../libs/esclib.h"

#include "../coredata.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>

char* GetStringFromUser(const char* title, Vector2i currPos, size_t* dumpSizeOfTo, bool* dumpIsDoneTo) {
	(void)title;
	(void)dumpIsDoneTo;
	(void)dumpSizeOfTo;

	ShowCursor();
	SetCursorPositionV(currPos);

	char ch = -1;
	
	char* tring = malloc(64 * sizeof(char));
	tring[0] = '\0';

	size_t tringIdx = 0;
	size_t tringCap = 64;

	(void)tringCap;

	Vector2i tringPos = currPos;
	
	while(1) {
		BeginDrawing();
		
		if(read(STDIN_FILENO, &ch, 1) == 1) {
			//if(ch == '\n') { break; } // REMAMBER, \n is different in raw mode. fix FIX:
			//if(ch == '\033') {  } // Mmmm, ESC. ESC lib :) TODO:

			tring[tringIdx++] = ch;
			tring[tringIdx] = '\0';
		}

		DrawTextV(tring, tringPos, TERMWHITE);
		if(ch == 'q') { exit(0); }

		EndDrawing();
	}
	BeginDrawing();

	return tring;
}
