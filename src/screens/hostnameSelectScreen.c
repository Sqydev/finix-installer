#include "../coredata.h"

#include "../screenModules/screenModules.h"

#include <stdlib.h>
#include <string.h>

void ScreenHostnameSelect(void) {
	DrawLineEx(" ", strlen("   Hostname:              "), 5, GetLastTuiIndex().x, 5, &TERMWHITE, &TERMBLACK, 1);

	char* tring = GetStringFromUser((Vector2i){ strlen("   Hostname:              "), 5 });	

	if(!tring) { DATA.screenState = SCREEN_MAIN; return; }
	if(tring[0] == '\0') {
		free(tring);

		DATA.Hostname.selected = false;
		DATA.Hostname.stringSizeOf = 0;
		DATA.Hostname.string = NULL;
		DATA.screenState = SCREEN_MAIN;

		return;
	}

	if(DATA.Hostname.string) {
		free(DATA.Hostname.string);
		DATA.Hostname.string = NULL;
	}

	DATA.Hostname.stringSizeOf = strlen(tring);
	DATA.Hostname.string = tring;
	DATA.Hostname.selected = true;

	DATA.screenState = SCREEN_MAIN;
}
