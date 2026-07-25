#include "../../libs/esclib.h"

#include "../coredata.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <fcntl.h>
#include <unistd.h>
#include <termios.h>

char* GetStringFromUser(const char* title, size_t* dumpSizeOfTo, bool* dumpIsDoneTo) {
	BeginDrawing();
	ClearTui(TERMBLACK, TERMWHITE);

	DrawTextf(title, 0, 0, TERMWHITE);

	EndDrawing();

	char* tring = NULL;
	*dumpSizeOfTo = 0;

	int previousTermFlags = fcntl(STDIN_FILENO, F_GETFL);
	fcntl(STDIN_FILENO, F_SETFL, previousTermFlags & ~O_NONBLOCK);

	struct termios oldTty, tty;
	tcgetattr(STDIN_FILENO, &oldTty);
	tty = oldTty;

	tty.c_lflag |= (ECHO | ICANON | ISIG);
	tty.c_iflag |= ICRNL;
	tty.c_cc[VMIN]  = 1;
	tty.c_cc[VTIME] = 0;
	tcsetattr(STDIN_FILENO, TCSANOW, &tty);
	tcflush(STDIN_FILENO, TCIFLUSH);

	ssize_t len = getline(&tring, dumpSizeOfTo, stdin);
	if(len == -1) {
		free(tring);
		*dumpIsDoneTo = false;
		tcsetattr(STDIN_FILENO, TCSANOW, &oldTty);
		fcntl(STDIN_FILENO, F_SETFL, previousTermFlags);
		return NULL;
	}
	if(len > 0 && tring[len - 1] == '\n') {
		tring[len - 1] = '\0';
	}

	tcsetattr(STDIN_FILENO, TCSANOW, &oldTty);
	fcntl(STDIN_FILENO, F_SETFL, previousTermFlags);

	*dumpIsDoneTo = true;
	DATA.redraw = true;

	BeginDrawing();
	ClearTui(TERMBLACK, TERMWHITE);
	return tring;
}
