#include "../coredata.h"

#include "../screenModules/screenModules.h"

const char* const SwapBackList[] = {
	"None",
	"Swap",
	"Zram"
};

size_t sizeOfSwapBackList = sizeof(SwapBackList) / sizeof(SwapBackList[0]);

void ScreenSwapSelect(void) {
	SelectFromListOnly("Select swap setup: ", SwapBackList, sizeOfSwapBackList, &DATA.Swap.selectedString, &DATA.Swap.selected);
}
