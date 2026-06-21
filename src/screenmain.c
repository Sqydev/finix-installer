#include "../libs/esclib.h"

#include "./coredata.h"

void drawScreenmain() {
	ClearTui(TERMBLACK, TERMWHITE);

	DrawText("Welcome to the finix installer", 0, 0, TERMWHITE);

	DrawTextf("Installer language:    %s", 3, 2, TERMWHITE);
	DrawTextf("Locales:               %s", 3, 3, TERMWHITE);
	DrawTextf("Partitions:            %s", 3, 4, TERMWHITE);
	DrawTextf("Bootloader:            %s", 3, 5, TERMWHITE);
	DrawTextf("Swap:                  %s", 3, 6, TERMWHITE);
	DrawTextf("Hostname:              %s", 3, 7, TERMWHITE);
	DrawTextf("Root password:         %s", 3, 8, TERMWHITE);
	DrawTextf("Users:                 %s", 3, 9, TERMWHITE);
	DrawTextf("Audio:                 %s", 3, 10, TERMWHITE);
	DrawTextf("Kernels:               %s", 3, 11, TERMWHITE);
	DrawTextf("Additional pkgs:       %s", 3, 12, TERMWHITE);
	DrawTextf("Timezone:              %s", 3, 13, TERMWHITE);

	DrawTextf("Premade config:        %s", 3, 14, TERMWHITE);

	DrawText("Install", 3, 4, TERMWHITE);
	DrawText("Abort", 3, 4, TERMWHITE);

	DrawChar(">", 1, DATA.cursorPos + 2, TERMWHITE);
}

void screenmain(void) {
	DATA.cursorPos = 0;

	drawScreenmain();

	while(1) {
		BeginDrawing();
		
		if(IsKeyPressed(KEY_W)) {
			DATA.cursorPos--;
			if(DATA.cursorPos < 0) { DATA.cursorPos = 0; }
		}
		if(IsKeyPressed(KEY_S)) {
			DATA.cursorPos++;
			if(DATA.cursorPos > 12) { DATA.cursorPos = 12; }
		}

		drawScreenmain();

		EndDrawing();
	}
}
