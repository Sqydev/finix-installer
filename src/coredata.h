#ifndef FINIX_INSTALLER_COREDATA_H
#define FINIX_INSTALLER_COREDATA_H

#include <stddef.h>

typedef enum {
	SCREEN_MAIN,
	SCREEN_TIMEZONE,
} Screen;

typedef struct {
	struct {
		const char* const* content;
		size_t linesCount;
		size_t selectedIndex;
		bool selected;
	} TimeZone;

	int cursorPos;
	Screen currScreen;
} CoreData;

extern CoreData DATA;

#endif
