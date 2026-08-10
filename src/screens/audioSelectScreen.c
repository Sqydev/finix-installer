#include "../coredata.h"

#include "../screenModules/screenModules.h"

const char* const AudioBackList[] = {
	"Pulseaudio",
	"Pipewire"
};

size_t sizeOfAudioBackList = sizeof(AudioBackList) / sizeof(AudioBackList[0]);

void ScreenAudioSelect(void) {
	SelectFromListOnly("Select audio backend: ", AudioBackList, sizeOfAudioBackList, &DATA.Audio.selectedString, &DATA.Audio.selected);
}
