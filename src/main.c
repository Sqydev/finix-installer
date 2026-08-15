#include "../libs/esclib.h"

#include "./screens/screens.h"
#include "./coredata.h"

#include <signal.h>
#include <stdlib.h>

CoreData DATA;

void RedrawSigHandler() {
	DATA.redraw = true;
}

void CleanUp(void) {
	if(DATA.Locale.custom && DATA.Locale.string && DATA.Locale.selected) { free(DATA.Locale.string); }

	if(DATA.Hostname.string && DATA.Hostname.selected) { free(DATA.Hostname.string); }

	if(DATA.Kernel.custom && DATA.Kernel.string && DATA.Kernel.selected) { free(DATA.Kernel.string); }

	if(DATA.Timezone.custom && DATA.Timezone.string && DATA.Timezone.selected) { free(DATA.Timezone.string); }

	for(size_t i = 0; i < DATA.Users.usersCount; i++) {
		if(DATA.Users.users[i].name) { free(DATA.Users.users[i].name); }
		if(DATA.Users.users[i].passwd) { free(DATA.Users.users[i].passwd); }
	 	for(size_t j = 0; j < DATA.Users.users[i].groupsCount; j++) {
			if(DATA.Users.users[i].groups[j]) { free(DATA.Users.users[i].groups[j]); }
	 	}
		if(DATA.Users.users[i].groups) { free(DATA.Users.users[i].groups); }
	}

	CloseTui();
}

int main(int argc, char** argv) {
	(void)argc;
	(void)argv;

	InitTui(20, TUI_DYNAMIC);

	AddSignalTask(SIGINT, CleanUp, -1);
	AddSignalTask(SIGWINCH, RedrawSigHandler, -1);

	HideCursor();

	DATA.screenState = SCREEN_MAIN;

	DATA.Locale.custom = false;
	DATA.Locale.selected = false;
	DATA.Locale.stringSizeOf = 0;
	DATA.Locale.string = NULL;

	DATA.Swap.selected = false;
	DATA.Swap.selectedString = NULL;

	DATA.Hostname.selected = false;
	DATA.Hostname.stringSizeOf = 0;
	DATA.Hostname.string = NULL;

	DATA.RootPasswd.selected = false;
	DATA.RootPasswd.stringSizeOf = 0;
	DATA.RootPasswd.string = NULL;

	DATA.Users.users = NULL;
	DATA.Users.usersCount = 0;

	DATA.Audio.selected = false;
	DATA.Audio.selectedString = NULL;

	DATA.Kernel.custom = false;
	DATA.Kernel.selected = false;
	DATA.Kernel.stringSizeOf = 0;
	DATA.Kernel.string = NULL;

	DATA.Timezone.custom = false;
	DATA.Timezone.selected = false;
	DATA.Timezone.stringSizeOf = 0;
	DATA.Timezone.string = NULL;

	DATA.InstallType.selected = false;
	DATA.InstallType.selectedString = NULL;

	DATA.selectedCheckList[0] = &DATA.Locale.selected;
	DATA.selectedCheckList[1] = &DATA.Swap.selected;
	DATA.selectedCheckList[2] = &DATA.Hostname.selected;
	DATA.selectedCheckList[3] = &DATA.RootPasswd.selected;
	DATA.selectedCheckList[4] = &DATA.Audio.selected;
	DATA.selectedCheckList[5] = &DATA.Kernel.selected;
	DATA.selectedCheckList[6] = &DATA.Timezone.selected;
	DATA.selectedCheckList[7] = &DATA.InstallType.selected;

	for(;;) {
		BeginDrawing();

		DATA.allOptionsSelected = true;
		for(int i = 0; i < selCheckListSize; i++) {
			if(!*DATA.selectedCheckList[i]) {
				DATA.allOptionsSelected = false;
				break;
			}
		}

		switch(DATA.screenState) {
			case SCREEN_MAIN: {
				ScreenMain();
				break;
			}
			case SCREEN_LOCALE: {
				ScreenLocaleSelect();
				break;
			}
			case SCREEN_SWAP: {
				ScreenSwapSelect();
				break;
			}
			case SCREEN_HOSTNAME: {
				ScreenHostnameSelect();
				break;
			}
			case SCREEN_ROOTPASSWD: {
				ScreenRootPasswdSelect();
				break;
			}
			case SCREEN_USERS: {
				ScreenUsers();
				break;
			}
			case SCREEN_AUDIO: {
				ScreenAudioSelect();
				break;
			}
			case SCREEN_KERNEL: {
				ScreenKernelSelect();
				break;
			}
			case SCREEN_TIMEZONE: {
				ScreenTimezone();
				break;
			}
			case SCREEN_INSTALLTYPE: {
				ScreenInstallType();
				break;
			}
			case SCREEN_EXIT: {
				goto end;
			}
		}

		EndDrawing();
	}

	end:

	CleanUp();

	return 0;
}
