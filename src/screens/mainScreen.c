#include "../../libs/esclib.h"

#include "../screenModules/screenModules.h"

#include "../coredata.h"

#define GREET_IDX 0

#define LOCALES_IDX 2
#define PARTITIONS_IDX 3
#define SWAP_IDX 4
#define HOSTNAME_IDX 5
#define ROOTPASSWD_IDX 6
#define USERS_IDX 7
#define AUDIO_IDX 8
#define KERNEL_IDX 9
#define TIMEZONES_IDX 10
#define INSTALLTYPE_IDX 11

#define CONTINUE_IDX 13
#define ABORT_IDX 14

void DrawScreenMain(void) {
	ClearTui(TERMBLACK, TERMWHITE);
	DrawText("Welcome to the finix installer", 0, GREET_IDX, TERMWHITE);

	DrawTextf("Locales:               %s", 3, LOCALES_IDX, TERMWHITE, (DATA.Locale.selected) ? DATA.Locale.string : "Not selected");
	DrawTextf("Partitioning:          %s", 3, PARTITIONS_IDX, TERMWHITE);
	DrawTextf("Swap:                  %s", 3, SWAP_IDX, TERMWHITE);
	DrawTextf("Hostname:              %s", 3, HOSTNAME_IDX, TERMWHITE, (DATA.Hostname.selected) ? DATA.Hostname.string : "Not selected");
	DrawTextf("Root password:         %s", 3, ROOTPASSWD_IDX, TERMWHITE, (DATA.RootPasswd.selected) ? DATA.RootPasswd.string : "Not selected");
	DrawTextf("Users:                 %s", 3, USERS_IDX, TERMWHITE);
	DrawTextf("Audio:                 %s", 3, AUDIO_IDX, TERMWHITE, (DATA.Audio.selected) ? DATA.Audio.selectedString : "Not selected");
	DrawTextf("Kernels:               %s", 3, KERNEL_IDX, TERMWHITE, (DATA.Kernel.selected) ? DATA.Kernel.string : "Not selected");
	DrawTextf("Timezone:              %s", 3, TIMEZONES_IDX, TERMWHITE, (DATA.Timezone.selected) ? DATA.Timezone.string : "Not selected");
	DrawTextf("Install type:          %s", 3, INSTALLTYPE_IDX, TERMWHITE, (DATA.InstallType.selected) ? DATA.InstallType.selectedString : "Not selected");

	DrawText("Continue", 3, CONTINUE_IDX, TERMWHITE);
	DrawText("Abort", 3, ABORT_IDX, TERMWHITE);
}

void ScreenMain(void) {
	static bool inited = false;
	if(!inited) {
		DATA.cursorPos = 2;
		inited = true;

		DrawScreenMain();
	}
	if(DATA.redraw) { DrawScreenMain(); DATA.redraw = false; }

	int Skip[1] = { 12 };
	HandleCursor(2, 14, Skip, 1);

	if(IsKeyPressed(KEY_ENTER)) {
		if(DATA.cursorPos == LOCALES_IDX) { DATA.screenState = SCREEN_LOCALE; }
		else if(DATA.cursorPos == HOSTNAME_IDX) { DATA.screenState = SCREEN_HOSTNAME; }
		else if(DATA.cursorPos == ROOTPASSWD_IDX) { DATA.screenState = SCREEN_ROOTPASSWD; }
		else if(DATA.cursorPos == AUDIO_IDX) { DATA.screenState = SCREEN_AUDIO; }
		else if(DATA.cursorPos == KERNEL_IDX) { DATA.screenState = SCREEN_KERNEL; }
		else if(DATA.cursorPos == TIMEZONES_IDX) { DATA.screenState = SCREEN_TIMEZONE; }
		else if(DATA.cursorPos == INSTALLTYPE_IDX) { DATA.screenState = SCREEN_INSTALLTYPE; }

		else if(DATA.cursorPos == ABORT_IDX) { DATA.screenState = SCREEN_EXIT; }
	}
	
	if(DATA.screenState != SCREEN_MAIN) { inited = false; }
}
