#ifndef FINIX_INSTALLER_COREDATA_H
#define FINIX_INSTALLER_COREDATA_H

typedef enum {
	SCREEN_EXIT,
	SCREEN_MAIN,
	SCREEN_TIMEZONE,
	SCREEN_INSTALLTYPE,
} Screen;

typedef struct {
	Screen screenState;
	int cursorPos;

	struct {
		char** selectedString;
		bool selected;
	} Timezone;
} CoreData;

extern CoreData DATA;

#endif
