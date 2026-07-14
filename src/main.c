#include "../libs/esclib.h"

#include "./screens/screens.h"
#include "./coredata.h"

#include <signal.h>

CoreData DATA;

void RedrawSigHandler() {
	DATA.redraw = true;
}

void CleanUp(void) {
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
	DATA.Timezone.selected = false;
	DATA.Timezone.selectedString = NULL;

	for(;;) {
		BeginDrawing();

		switch(DATA.screenState) {
			case SCREEN_MAIN: {
				ScreenMain();
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
