#include "../../libs/esclib.h"

#include "../screenModules/screenModules.h"

#include "../coredata.h"

void DrawScreenMain(void) {
	ClearTui(TERMBLACK, TERMWHITE);
	DrawText("Welcome to the finix installer", 0, GREET_IDX, TERMWHITE);

	DrawTextf("Locales:               %s", 3, LOCALES_IDX, TERMWHITE, (DATA.Locale.selected) ? DATA.Locale.string : "Not selected");
	DrawTextf("Partitioning:            ", 3, PARTITIONS_IDX, TERMWHITE);
	DrawTextf("Swap:                  %s", 3, SWAP_IDX, TERMWHITE, (DATA.Swap.selected) ? DATA.Swap.selectedString : "Not selected");
	DrawTextf("Hostname:              %s", 3, HOSTNAME_IDX, TERMWHITE, (DATA.Hostname.selected) ? DATA.Hostname.string : "Not selected");
	DrawTextf("Root password:         %s", 3, ROOTPASSWD_IDX, TERMWHITE, (DATA.RootPasswd.selected) ? DATA.RootPasswd.string : "Not selected");
	DrawTextf("Users:                 %s", 3, USERS_IDX, TERMWHITE, (DATA.Users.usersCount > 0) ? "Done" : "Not selected");
	DrawTextf("Audio:                 %s", 3, AUDIO_IDX, TERMWHITE, (DATA.Audio.selected) ? DATA.Audio.selectedString : "Not selected");
	DrawTextf("Kernels:               %s", 3, KERNEL_IDX, TERMWHITE, (DATA.Kernel.selected) ? DATA.Kernel.string : "Not selected");
	DrawTextf("Timezone:              %s", 3, TIMEZONES_IDX, TERMWHITE, (DATA.Timezone.selected) ? DATA.Timezone.string : "Not selected");
	DrawTextf("Install type:          %s", 3, INSTALLTYPE_IDX, TERMWHITE, (DATA.InstallType.selected) ? DATA.InstallType.selectedString : "Not selected");

	if(!DATA.allOptionsSelected) { DrawText("Continue", 3, CONTINUE_IDX, (Color){ 7, 0, 0, 1, false }); }
	else { DrawText("Continue", 3, CONTINUE_IDX, TERMWHITE); }
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
		else if(DATA.cursorPos == PARTITIONS_IDX) { DATA.screenState = SCREEN_PARTITIONS; }
		else if(DATA.cursorPos == SWAP_IDX) { DATA.screenState = SCREEN_SWAP; }
		else if(DATA.cursorPos == HOSTNAME_IDX) { DATA.screenState = SCREEN_HOSTNAME; }
		else if(DATA.cursorPos == ROOTPASSWD_IDX) { DATA.screenState = SCREEN_ROOTPASSWD; }
		else if(DATA.cursorPos == USERS_IDX) { DATA.screenState = SCREEN_USERS; }
		else if(DATA.cursorPos == AUDIO_IDX) { DATA.screenState = SCREEN_AUDIO; }
		else if(DATA.cursorPos == KERNEL_IDX) { DATA.screenState = SCREEN_KERNEL; }
		else if(DATA.cursorPos == TIMEZONES_IDX) { DATA.screenState = SCREEN_TIMEZONE; }
		else if(DATA.cursorPos == INSTALLTYPE_IDX) { DATA.screenState = SCREEN_INSTALLTYPE; }

		else if(DATA.cursorPos == CONTINUE_IDX && DATA.allOptionsSelected) { DATA.screenState = SCREEN_EXIT; }
		else if(DATA.cursorPos == ABORT_IDX) { DATA.screenState = SCREEN_EXIT; }
	}
	
	if(DATA.screenState != SCREEN_MAIN) { inited = false; }
}
