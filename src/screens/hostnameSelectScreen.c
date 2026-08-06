#include "../coredata.h"

#include "../screenModules/screenModules.h"

#include <stdlib.h>
#include <string.h>

void ScreenHostnameSelect(void) {
	char* tring = GetStringFromUser((Vector2i){ GetCursorPos().x, GetCursorPos().y + strlen("Hostname:              ") });	

	if(!tring) { return; }

	if(DATA.Hostname.string) {
		free(DATA.Hostname.string);
		DATA.Hostname.string = NULL;
	}

	DATA.Hostname.stringSizeOf = strlen(tring);
	DATA.Hostname.string = tring;
	DATA.Hostname.selected = true;
}
