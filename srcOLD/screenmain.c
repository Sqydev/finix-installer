#include "../libs/esclib.h"

#include "./coredata.h"
#include "fns.h"

void DoScreenmain() {
	static bool inited = false;
	if(!inited) { DATA.cursorPos = 2; inited = true; }

	int Skip[2] = { 14, 16 };
	HandleCursor(2, 18, Skip, 2);

	if(IsKeyPressed(KEY_ENTER)) {
		if(DATA.cursorPos == 12) { DATA.currScreen = SCREEN_TIMEZONE; }
		else if(DATA.cursorPos == 13) { DATA.currScreen = SCREEN_INSTALLTYPE; }
	}
		
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
	DrawTextf("Timezone:              %s", 3, 12, TERMWHITE, (DATA.TimeZone.selected) ? DATA.TimeZone.content[DATA.TimeZone.selectedIndex] : "Not selected");
	DrawTextf("Install type:          %s", 3, 13, TERMWHITE, DATA.InstallTypes.strings[DATA.InstallTypes.selectedIndex]);

	DrawTextf("Premade config:        %s", 3, 15, TERMWHITE);

	DrawText("Install", 3, 17, TERMWHITE);
	DrawText("Abort", 3, 18, TERMWHITE);

	if(DATA.currScreen != SCREEN_MAIN) { inited = false; }
}

void screenmain(void) {
	DATA.cursorPos = 2;
	DATA.currScreen = SCREEN_MAIN;
	DATA.InstallTypes.selectedIndex = 0;

	while(1) {
		BeginDrawing();
		ClearTui(TERMBLACK, TERMWHITE);

		switch(DATA.currScreen) {
			case SCREEN_MAIN: {
				DoScreenmain();
				break;
			}
			case SCREEN_TIMEZONE: {
				SelectFromList("Select timezone", DATA.TimeZone.content, DATA.TimeZone.linesCount, &DATA.TimeZone.selectedIndex, &DATA.TimeZone.selected);
				break;
			}
			case SCREEN_INSTALLTYPE: {
				SelectFromList("Select install type", DATA.InstallTypes.strings, DATA.InstallTypes.sizeOfStrings, &DATA.InstallTypes.selectedIndex, NULL);
				break;
			}
		}

		EndDrawing();
	}
}
