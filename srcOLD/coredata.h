#ifndef FINIX_INSTALLER_COREDATA_H
#define FINIX_INSTALLER_COREDATA_H

#include <stddef.h>
#include <stdbool.h>

typedef enum {
	SCREEN_MAIN,
	SCREEN_TIMEZONE,
	SCREEN_INSTALLTYPE,
} Screen;

typedef struct {
	struct {
		const char* const* content;
		size_t linesCount;
		size_t selectedIndex;
		bool selected;
	} TimeZone;

	struct {
		const char* const* strings;
		size_t selectedIndex;
		size_t sizeOfStrings;
	} InstallTypes;

	int cursorPos;
	Screen currScreen;
} CoreData;

extern CoreData DATA;

#endif
