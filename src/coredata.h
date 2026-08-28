#ifndef FINIX_INSTALLER_COREDATA_H
#define FINIX_INSTALLER_COREDATA_H

#include <stdbool.h>
#include <stddef.h>


#define GREET_IDX 0

#define LOCALES_IDX 2
#define PARTITIONS_IDX 3
#define SWAP_IDX 4
#define HOSTNAME_IDX 5
#define ROOTPASSWD_IDX 6
#define USERS_IDX 7
#define AUDIO_IDX 8
#define KERNEL_IDX 9
#define TIMEZONES_IDX 10
#define INSTALLTYPE_IDX 11

#define CONTINUE_IDX 13
#define ABORT_IDX 14

typedef enum {
	SCREEN_EXIT,
	SCREEN_MAIN,
	SCREEN_LOCALE,
	SCREEN_SWAP,
	SCREEN_HOSTNAME,
	SCREEN_ROOTPASSWD,
	SCREEN_USERS,
	SCREEN_AUDIO,
	SCREEN_KERNEL,
	SCREEN_TIMEZONE,
	SCREEN_INSTALLTYPE,
} Screen;

typedef struct {
	char* name;
	char* passwd;
	bool isNormalUser;

	char** groups;
	char* groupsRaw;
	size_t groupsCount;
} User;

typedef struct {
	Screen screenState;
	int cursorPos;
	bool redraw;

	#define selCheckListSize 9
	bool* selectedCheckList[selCheckListSize];
	bool allOptionsSelected;

	struct {
		char* string;
		bool selected;
		size_t stringSizeOf;
		bool custom;
	} Locale;
	struct {
		char* selectedString;
		bool selected;
	} Swap;
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
		User* users;
		size_t usersCount;
		bool done;
	} Users;
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
