#include "../libs/esclib.h"

#include "./screens/screens.h"
#include "./coredata.h"

#include <signal.h>

CoreData DATA;

void CleanUp(void) {
	CloseTui();
}

int main(int argc, char** argv) {
	(void)argc;
	(void)argv;

	InitTui(20, TUI_DYNAMIC);

	AddSignalTask(SIGINT, CleanUp, -1);

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
				break;
			}
			case SCREEN_INSTALLTYPE: {
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
