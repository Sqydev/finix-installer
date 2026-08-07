#ifndef FINIX_INSTALLER_COREDATA_H
#define FINIX_INSTALLER_COREDATA_H

#include <stdbool.h>
#include <stddef.h>

typedef enum {
	SCREEN_EXIT,
	SCREEN_MAIN,
	SCREEN_HOSTNAME,
	SCREEN_ROOTPASSWD,
	SCREEN_AUDIO,
	SCREEN_KERNEL,
	SCREEN_TIMEZONE,
	SCREEN_INSTALLTYPE,
} Screen;

typedef struct {
	Screen screenState;
	int cursorPos;
	bool redraw;
	char gotCh;

	struct {
		char* string;
		bool selected;
		size_t stringSizeOf;
	} Hostname;
	struct {
		char* string;
		bool selected;
		size_t stringSizeOf;
	} RootPasswd;
	struct {
		char* selectedString;
		bool selected;
	} Audio;
	struct {
		char* string;
		bool selected;
		size_t stringSizeOf;
		bool custom;
	} Kernel;
	struct {
		char* string;
		bool selected;
		size_t stringSizeOf;
		bool custom;
	} Timezone;
	struct {
		char* selectedString;
		bool selected;
	} InstallType;
} CoreData;

extern CoreData DATA;

#endif
