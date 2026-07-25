#include "../coredata.h"

#include "../screenModules/screenModules.h"
#include <string.h>

const char* const KernelsList[] = {
    "Custom: ",
    "pkgs.linuxPackages",
    "pkgs.linuxPackages_5_10",
    "pkgs.linuxPackages_5_10_hardened",
    "pkgs.linuxPackages_5_15",
    "pkgs.linuxPackages_5_15_hardened",
    "pkgs.linuxPackages_5_4",
    "pkgs.linuxPackages_5_4_hardened",
    "pkgs.linuxPackages_6_1",
    "pkgs.linuxPackages_6_12",
    "pkgs.linuxPackages_6_12_hardened",
    "pkgs.linuxPackages_6_13",
    "pkgs.linuxPackages_6_13_hardened",
    "pkgs.linuxPackages_6_14",
    "pkgs.linuxPackages_6_14_hardened",
    "pkgs.linuxPackages_6_15",
    "pkgs.linuxPackages_6_16",
    "pkgs.linuxPackages_6_17",
    "pkgs.linuxPackages_6_18",
    "pkgs.linuxPackages_6_19",
    "pkgs.linuxPackages_6_1_hardened",
    "pkgs.linuxPackages_6_6",
    "pkgs.linuxPackages_6_6_hardened",
    "pkgs.linuxPackages_7_0",
    "pkgs.linuxPackages_7_1",
    "pkgs.linuxPackages_custom",
    "pkgs.linuxPackages_custom_tinyconfig_kernel",
    "pkgs.linuxPackages_ham",
    "pkgs.linuxPackages_hardened",
    "pkgs.linuxPackages_latest",
    "pkgs.linuxPackages_latest-libre",
    "pkgs.linuxPackages_latest_xen_dom0",
    "pkgs.linuxPackages_lqx",
    "pkgs.linuxPackages_rpi0",
    "pkgs.linuxPackages_rpi02w",
    "pkgs.linuxPackages_rpi1",
    "pkgs.linuxPackages_rpi2",
    "pkgs.linuxPackages_rpi3",
    "pkgs.linuxPackages_rpi4",
    "pkgs.linuxPackages_rt_5_10",
    "pkgs.linuxPackages_rt_5_15",
    "pkgs.linuxPackages_rt_5_4",
    "pkgs.linuxPackages_rt_6_1",
    "pkgs.linuxPackages_testing",
    "pkgs.linuxPackages_xanmod",
    "pkgs.linuxPackages_xanmod_latest",
    "pkgs.linuxPackages_xanmod_stable",
    "pkgs.linuxPackages_xen_dom0",
    "pkgs.linuxPackages_xen_dom0_hardened",
    "pkgs.linuxPackages_zen"
};

size_t sizeOfKernelsList = sizeof(KernelsList) / sizeof(KernelsList[0]);

void ScreenKernelSelect(void) {
	SelectFromConstList("Select kernel package to be installed", KernelsList, sizeOfKernelsList, &DATA.Kernel.string, &DATA.Kernel.selected);
	if(DATA.Kernel.selected) { DATA.Kernel.stringSizeOf = strlen(DATA.Kernel.string); }
	if(DATA.Kernel.string == KernelsList[0]) {
		DATA.Kernel.string = GetStringFromUser("Type", &DATA.Kernel.stringSizeOf, &DATA.Kernel.selected);
	}
}
