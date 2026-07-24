#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <fcntl.h>
#include <unistd.h>
#include <termios.h>

void GetStringFromUser(const char* title, char** dumpStringTo, bool* dumpIsStringDone) {
	(void)title;

	if(!dumpStringTo) { return; }

	int defaultTermFlags = fcntl(STDIN_FILENO, F_GETFL);
    fcntl(STDIN_FILENO, F_SETFL, defaultTermFlags & ~O_NONBLOCK);
	struct termios tty;
    tcgetattr(STDIN_FILENO, &tty);
    tty.c_lflag |= ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &tty);

	*dumpIsStringDone = false;
	
	if(*dumpStringTo != NULL) { free(*dumpStringTo); *dumpStringTo = NULL; }

	scanf("%s", *dumpStringTo);

	*dumpIsStringDone = true;

    tcgetattr(STDIN_FILENO, &tty);
    tty.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &tty);
    fcntl(STDIN_FILENO, F_SETFL, defaultTermFlags | O_NONBLOCK);
}
