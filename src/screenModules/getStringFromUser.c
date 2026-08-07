#include "../../libs/esclib.h"

#include "../coredata.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>

#define TRING_INITSIZE 64

char* GetStringFromUser(Vector2i currPos) {
	ShowCursor();
	SetCursorPositionV(currPos);

	EndDrawing();

	char* tring = malloc(TRING_INITSIZE * sizeof(char));
	tring[0] = '\0';

	size_t tringIdx = 0;
	size_t tringCap = 64;

	Vector2i tringPos = currPos;
	
	while(1) {
		BeginDrawing();

		EscKey gotKey = WaitForKeyPress();
		if(gotKey != KEY_NULL) {
			if(gotKey == KEY_ENTER) { break; }
			if(gotKey == KEY_ESCAPE) { free(tring); tring = NULL; break; }

			if(tringCap >= tringIdx) {
				tring = realloc(tring, (tringCap + 1) * sizeof(char));
				if(!tring) { break; }

				tringCap++;
			}

			tring[tringIdx++] = gotKey;
			tring[tringIdx] = '\0';

			currPos.x++;
		}

		SetCursorPositionV(currPos);
		
		DrawTextV(tring, tringPos, TERMWHITE);

		EndDrawing();
	}
	BeginDrawing();

	HideCursor();

	return tring;
}
