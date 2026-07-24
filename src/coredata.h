#ifndef FINIX_INSTALLER_COREDATA_H
#define FINIX_INSTALLER_COREDATA_H

#include <stdbool.h>

typedef enum {
	SCREEN_EXIT,
	SCREEN_MAIN,
	SCREEN_KERNEL,
	SCREEN_TIMEZONE,
	SCREEN_INSTALLTYPE,
} Screen;

typedef struct {
	Screen screenState;
	int cursorPos;
	bool redraw;

	struct {
		char* string;
		bool selected;
	} Kernel;
	struct {
		char* selectedString;
		bool selected;
	} Timezone;
	struct {
		char* selectedString;
		bool selected;
	} InstallType;
} CoreData;

extern CoreData DATA;

#endif
