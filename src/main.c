#include "../libs/esclib.h"

#include "./fns.h"
#include "./coredata.h"

#include <signal.h>

CoreData DATA;

int main(int argc, char** argv) {
	(void)argc;
	(void)argv;

	InitTui(20, TUI_DYNAMIC);

	AddSignalTask(SIGINT, CleanUp, -1);

	HideCursor();

	DATA.TimeZone.selectedIndex = 0;
	DATA.TimeZone.selected = false;
	DATA.TimeZone.content = GetTimeZones(&DATA.TimeZone.linesCount);
	DATA.InstallTypes.strings = GetInstallTypes(&DATA.InstallTypes.sizeOfStrings);

	screenmain();

	CloseTui();

	CleanUp();

	return 0;
}
