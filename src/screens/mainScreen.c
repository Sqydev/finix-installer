#include "../../libs/esclib.h"

#include "../screenModules/screenModules.h"

#include "../coredata.h"

void DrawScreenMain(void) {
	ClearTui(TERMBLACK, TERMWHITE);
	DrawText("Welcome to the finix installer", 0, 0, TERMWHITE);

	DrawTextf("Locales:               %s", 3, 2, TERMWHITE);
	DrawTextf("Partitions:            %s", 3, 3, TERMWHITE);
	DrawTextf("Swap:                  %s", 3, 4, TERMWHITE);
	DrawTextf("Hostname:              %s", 3, 5, TERMWHITE);
	DrawTextf("Root password:         %s", 3, 6, TERMWHITE);
	DrawTextf("Users:                 %s", 3, 7, TERMWHITE);
	DrawTextf("Audio:                 %s", 3, 8, TERMWHITE);
	DrawTextf("Kernels:               %s", 3, 9, TERMWHITE);
	DrawTextf("Timezone:              %s", 3, 10, TERMWHITE, (DATA.Timezone.selected) ? DATA.Timezone.selectedString : "Not selected");
	DrawTextf("Install type:          %s", 3, 11, TERMWHITE, (DATA.InstallType.selected) ? DATA.InstallType.selectedString : "Not selected");

	DrawTextf("Premade config:        %s", 3, 13, TERMWHITE);
	DrawTextf("Edit config:           %s", 3, 14, TERMWHITE);

	DrawText("Install", 3, 16, TERMWHITE);
	DrawText("Abort", 3, 17, TERMWHITE);
}

void ScreenMain(void) {
	static bool inited = false;
	if(!inited) {
		DATA.cursorPos = 2;
		inited = true;

		DrawScreenMain();
	}

	int Skip[2] = { 12, 15 };
	HandleCursor(2, 17, Skip, 2);

	if(IsKeyPressed(KEY_ENTER)) {
		if(DATA.cursorPos == 10) { DATA.screenState = SCREEN_TIMEZONE; }
		else if(DATA.cursorPos == 11) { DATA.screenState = SCREEN_INSTALLTYPE; }

		else if(DATA.cursorPos == 17) { DATA.screenState = SCREEN_EXIT; }
	}
	
	if(DATA.screenState != SCREEN_MAIN) { inited = false; }
}
