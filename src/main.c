#include "../libs/esclib.h"

#include "./fns.h"
#include "./coredata.h"

CoreData DATA;

int main(int argc, char** argv) {
	(void)argc;
	(void)argv;

	InitTui(20, TUI_DYNAMIC);

	HideCursor();

	screenmain();

	CloseTui();

	return 0;
}
