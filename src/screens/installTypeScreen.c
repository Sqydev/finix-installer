#include "../coredata.h"

#include "../screenModules/screenModules.h"

const char* const InstallTypeList[] = {
	"Channels based",
	"Flake based"
};

size_t sizeOfInstallTypeList = sizeof(InstallTypeList) / sizeof(InstallTypeList[0]);

void ScreenInstallType(void) {
	SelectFromConstList("Select instalation type", InstallTypeList, sizeOfInstallTypeList, &DATA.InstallType.selectedString, &DATA.InstallType.selected);
}
