#include "../../libs/esclib.h"
#include "../coredata.h"
#include "../screenModules/screenModules.h"

#include <stdlib.h>
#include <string.h>

typedef enum {
	TAMPRING_USER_ADD,
	TAMPRING_USER_EDIT,
	TAMPRING_NONE
} UserTampringType;

void UserTampringScreen(UserTampringType* state, size_t idx) {
	ClearTui(TERMBLACK, TERMWHITE);

	bool isAdd = (*state == TAMPRING_USER_ADD);

	int skip[1] = { 5 };
	HandleCursor(2, 7, skip, 1);

	DrawText(isAdd ? "Add User: " : "Edit User: ", 0, 0, TERMWHITE);

	DrawTextf("Username:     %s", 3, 2, TERMWHITE, DATA.Users.users[idx].name);
	DrawTextf("Password:     %s", 3, 3, TERMWHITE, DATA.Users.users[idx].passwd);
	DrawTextf("Groups:       %s", 3, 4, TERMWHITE, DATA.Users.users[idx].groupsRaw);

	DrawText(isAdd ? "Add" : "Save", 3, 6, TERMWHITE);
	DrawText("DELETE!!!", 3, 7, TERMWHITE);

	if(IsKeyPressed(KEY_ESCAPE)) {
		if(isAdd) {
			if(DATA.Users.users[idx].name) { free(DATA.Users.users[idx].name); }
			if(DATA.Users.users[idx].passwd) { free(DATA.Users.users[idx].passwd); }
			if(DATA.Users.users[idx].groupsRaw) { free(DATA.Users.users[idx].groupsRaw); }
		}
		*state = TAMPRING_NONE;
		DATA.cursorPos = 2;
		return;
	}

	if(IsKeyPressed(KEY_ENTER)) {
		switch(DATA.cursorPos) {
			case 2: {
				DrawLineEx(" ", strlen("   Username:     "), 2, GetLastTuiIndex().x, 5, &TERMWHITE, &TERMBLACK, 1);
				char* tring = GetStringFromUser((Vector2i){ 17, DATA.cursorPos });
				
				if(!tring) { return; }

				if(tring[0] != '\n') {
					if(DATA.Users.users[idx].name) { free(DATA.Users.users[idx].name); }
					DATA.Users.users[idx].name = tring;
				}
				else { free(tring); DATA.Users.users[idx].name[0] = '\0'; }
				break;
			}
			case 3: {
				DrawLineEx(" ", strlen("   Password:     "), 3, GetLastTuiIndex().x, 5, &TERMWHITE, &TERMBLACK, 1);
				char* tring = GetStringFromUser((Vector2i){ 17, DATA.cursorPos });
				
				if(!tring) { return; }

				if(tring[0] != '\n') { 
					if(DATA.Users.users[idx].passwd) { free(DATA.Users.users[idx].passwd); }
					DATA.Users.users[idx].passwd = tring;
				}
				else { free(tring); DATA.Users.users[idx].passwd[0] = '\0'; }
				break;
			}
			case 4: {
				DrawLineEx(" ", strlen("   Groups:     "), 4, GetLastTuiIndex().x, 5, &TERMWHITE, &TERMBLACK, 1);
				char* tring = GetStringFromUser((Vector2i){ 17, DATA.cursorPos });
				
				if(!tring) { return; }

				if(tring[0] != '\n') { 
					if(DATA.Users.users[idx].groupsRaw) { free(DATA.Users.users[idx].groupsRaw); }
					DATA.Users.users[idx].groupsRaw = tring;
				}
				else { free(tring); DATA.Users.users[idx].groupsRaw[0] = '\0'; }
				break;
			}
			case 6: {
				char* groupsCopy = strdup(DATA.Users.users[idx].groupsRaw);
				char* tok = strtok(groupsCopy, " ");
				
				for(size_t i = 0; tok; i++) {
					if(i >= DATA.Users.users[idx].groupsCount) { 
						DATA.Users.users[idx].groups = realloc(DATA.Users.users[idx].groups, sizeof(char*) * (DATA.Users.users[idx].groupsCount + 1)); 
						DATA.Users.users[idx].groupsCount++;
					}
					
					DATA.Users.users[idx].groups[i] = strdup(tok);
					if(!DATA.Users.users[idx].groups[i]) { 
						free(groupsCopy); 
						goto skipToDELETE; 
					}

					tok = strtok(NULL, " ");
				}
				free(groupsCopy);

				if(isAdd) { DATA.Users.usersCount++; }
				if(DATA.Users.usersCount > 0) { DATA.Users.done = true; }
				*state = TAMPRING_NONE;
				DATA.cursorPos = 2;
				break;
			}
			case 7: {
				skipToDELETE:
				if(DATA.Users.users[idx].name) { free(DATA.Users.users[idx].name); }
				if(DATA.Users.users[idx].passwd) { free(DATA.Users.users[idx].passwd); }
				if(DATA.Users.users[idx].groupsRaw) { free(DATA.Users.users[idx].groupsRaw); }
				for(size_t j = 0; j < DATA.Users.users[idx].groupsCount; j++) {
					if(DATA.Users.users[idx].groups[j]) { free(DATA.Users.users[idx].groups[j]); }
				}
				if(DATA.Users.users[idx].groups) { free(DATA.Users.users[idx].groups); }

				if(!isAdd) {
					for(size_t i = idx; i + 1 < DATA.Users.usersCount; i++) {
						DATA.Users.users[i] = DATA.Users.users[i + 1];
					}
					DATA.Users.usersCount--;
				}

				if(DATA.Users.usersCount <= 0) { DATA.Users.done = false; }

				*state = TAMPRING_NONE;
				DATA.cursorPos = 2;
				break;
			}
		}
	}
}

void ScreenUsers(void) {
	static bool inited = false;
	static UserTampringType tampringState = TAMPRING_NONE;
	static size_t tampringIdx = 0;
	if(!inited) {
		DATA.cursorPos = 2;
		inited = true;
	}
	if(tampringState != TAMPRING_NONE) { UserTampringScreen(&tampringState, tampringIdx); return; }

	ClearTui(TERMBLACK, TERMWHITE);

	const int skip[1] = { 3 };
	HandleCursor(2, (DATA.Users.usersCount) ? 3 + DATA.Users.usersCount : 2, skip, 1);

	DrawText("Users: ", 0, 0, TERMWHITE);
	DrawText("Add User", 3, 2, TERMWHITE);

	for(size_t i = 0; i < DATA.Users.usersCount; i++) {
		DrawTextf("%s", 3, i + 4, TERMWHITE, DATA.Users.users[i].name);
	}

	if(IsKeyPressed(KEY_ESCAPE)) { 
		inited = false; 
		DATA.screenState = SCREEN_MAIN; 
	}
	
	if(IsKeyPressed(KEY_ENTER)) {
		if(DATA.cursorPos == 2) {
			tampringState = TAMPRING_USER_ADD;
			tampringIdx = DATA.Users.usersCount;
			DATA.Users.users = realloc(DATA.Users.users, sizeof(User) * (DATA.Users.usersCount + 1));
			
			DATA.Users.users[DATA.Users.usersCount].name = malloc(1); DATA.Users.users[DATA.Users.usersCount].name[0] = '\0';
			DATA.Users.users[DATA.Users.usersCount].passwd = malloc(1); DATA.Users.users[DATA.Users.usersCount].passwd[0] = '\0';
			DATA.Users.users[DATA.Users.usersCount].groupsRaw = malloc(1); DATA.Users.users[DATA.Users.usersCount].groupsRaw[0] = '\0';
			DATA.Users.users[DATA.Users.usersCount].groupsCount = 0;
			DATA.Users.users[DATA.Users.usersCount].groups = NULL;
		} else if(DATA.cursorPos >= 4 && DATA.cursorPos < (int)(4 + DATA.Users.usersCount)) {
			tampringState = TAMPRING_USER_EDIT;
			tampringIdx = DATA.cursorPos - 4;
		}
	}
}
