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
	if(DATA.Kernel.custom && DATA.Kernel.string && DATA.Kernel.selected) { free(DATA.Kernel.string); }

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

	DATA.Kernel.custom = false;
	DATA.Kernel.selected = false;
	DATA.Kernel.stringSizeOf = 0;
	DATA.Kernel.string = NULL;

	DATA.Timezone.selected = false;
	DATA.Timezone.selectedString = NULL;

	DATA.InstallType.selected = false;
	DATA.InstallType.selectedString = NULL;
	
	for(;;) {
		BeginDrawing();

		switch(DATA.screenState) {
			case SCREEN_MAIN: {
				ScreenMain();
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
