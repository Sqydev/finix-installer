#include "../coredata.h"

#include "../screenModules/screenModules.h"

#include <stdlib.h>
#include <string.h>

void ScreenRootPasswdSelect(void) {
	DrawLineEx(" ", strlen("   Root password:         "), 6, GetLastTuiIndex().x, 6, &TERMWHITE, &TERMBLACK, 1);

	char* tring = GetStringFromUser((Vector2i){ strlen("   Root password:         "), 6 });	

	if(!tring) { DATA.screenState = SCREEN_MAIN; return; }
	if(tring[0] == '\0') {
		free(tring);

		DATA.RootPasswd.selected = false;
		DATA.RootPasswd.stringSizeOf = 0;
		DATA.RootPasswd.string = NULL;
		DATA.screenState = SCREEN_MAIN;

		return;
	}

	if(DATA.RootPasswd.string) {
		free(DATA.RootPasswd.string);
		DATA.RootPasswd.string = NULL;
	}

	DATA.RootPasswd.stringSizeOf = strlen(tring);
	DATA.RootPasswd.string = tring;
	DATA.RootPasswd.selected = true;

	DATA.screenState = SCREEN_MAIN;
}
