#include "../libs/esclib.h"

void drawScreenmain() {
	BeginDrawing();
	ClearTui(TERMBLACK, TERMWHITE);

	DrawText("Welcome to the finix installer", 0, 0, TERMWHITE);

	DrawTextf("Installer language:    %s", 3, 2, TERMWHITE);
	DrawTextf("Locales:               %s", 3, 3, TERMWHITE);
	DrawTextf("Partitions:            %s", 3, 4, TERMWHITE);
	DrawTextf("Bootloader:            %s", 3, 5, TERMWHITE);
	DrawTextf("Swap:                  %s", 3, 6, TERMWHITE);
	DrawTextf("Hostname:              %s", 3, 7, TERMWHITE);
	DrawTextf("Root password:         %s", 3, 8, TERMWHITE);
	DrawTextf("Users:                 %s", 3, 9, TERMWHITE);
	DrawTextf("Audio:                 %s", 3, 10, TERMWHITE);
	DrawTextf("Kernels:               %s", 3, 11, TERMWHITE);
	DrawTextf("Additional pkgs:       %s", 3, 12, TERMWHITE);
	DrawTextf("Timezone:              %s", 3, 13, TERMWHITE);

	DrawTextf("Premade config:        %s", 3, 14, TERMWHITE);

	DrawText("Install", 3, 4, TERMWHITE);
	DrawText("Abort", 3, 4, TERMWHITE);

	EndDrawing();
}

void screenmain(void) {
	drawScreenmain();

	while(1) {
	}
}
