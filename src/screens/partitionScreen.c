#include "../coredata.h"

#include "../screenModules/screenModules.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <dirent.h>
#include <limits.h>
#include <sys/wait.h>

char** find_disks(size_t *count) {
	DIR* dir = opendir("/sys/block");
	if(!dir) {
		return NULL;
	}

	char** disks = NULL;
	size_t n = 0;

	struct dirent* entry;

	while((entry = readdir(dir)) != NULL) {
		if(entry->d_name[0] == '.') {
			continue;
		}

		char device_path[PATH_MAX];
		snprintf(device_path, sizeof(device_path), "/sys/block/%s/device", entry->d_name);
		if(access(device_path, F_OK) != 0) {
			continue;
		}

		char path[PATH_MAX];
		snprintf(path, sizeof(path), "/dev/%s", entry->d_name);

		char** tmp = realloc(disks, (n + 1) * sizeof(char*));
		if(!tmp) {
			for(size_t i = 0; i < n; i++) { free(disks[i]); }

			free(disks);
			closedir(dir);
			return NULL;
		}

		disks = tmp;

		disks[n] = strdup(path);
		if(!disks[n]) {
			for(size_t i = 0; i < n; i++) { free(disks[i]); }

			free(disks);
			closedir(dir);
			return NULL;
		}

		n++;
	}

	closedir(dir);

	*count = n;
	return disks;
}

void ScreenPartitions(void) {
	static bool inited = false;

	static size_t disksCount = 0;
	static char** disks = NULL;
	if(!inited) {
		disks = find_disks(&disksCount);

		inited = true;
	}

	char* currDisk = NULL;
	size_t sizeOfCurrDiskString = 0;
	bool selected = false;
	bool isCustom = false;

	SelectFromList("Select disk", "Custom: ", (const char**)disks, disksCount, &currDisk, &selected, &sizeOfCurrDiskString, &isCustom);
	if(!selected || !currDisk) {
		goto end;
	}

	EndFrame();
	CloseTui();

	pid_t pid = fork();

	if(pid == -1) {
		goto end;
	}

	if(pid == 0) {
		char* args[] = {
			"cfdisk",
			currDisk,
			NULL
		};

		execvp("cfdisk", args);

		exit(EXIT_FAILURE);
	}

	int status;
	if(waitpid(pid, &status, 0) == -1) {
		goto end;
	}

	InitTui(20, TUI_DYNAMIC);
	BeginFrame();

end:
	if(DATA.screenState != SCREEN_PARTITIONS) {
		for(size_t i = 0; i < disksCount; i++) { if(disks[i]) { free(disks[i]); } }
		if(disks) { free(disks); }
		disks = NULL;
		disksCount = 0;

		inited = false;
	}
}
