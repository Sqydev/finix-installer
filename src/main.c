#include "../libs/esclib.h"
#include "fns.h"

int main(int argc, char** argv) {
	(void)argc;
	(void)argv;

	InitTui(60, TUI_DYNAMIC);

	HideCursor();

	screenmain();

	CloseTui();

	return 0;
}
