#include "../coredata.h"

#include "../screenModules/screenModules.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <errno.h>
#include <dirent.h>
#include <limits.h>
#include <sys/wait.h>
#include <sys/stat.h>

typedef enum {
	PARTITION_SCREEN_SELECT_DISK,
	PARTITION_SCREEN_LIST_PARTITIONS,
	PARTITION_SCREEN_EDIT_PARTITION
} PartitionScreenPhase;

static const char* const FilesystemList[] = {
	"ext4",
	"btrfs",
	"xfs",
	"f2fs",
	"vfat",
	"swap"
};

static const size_t FilesystemCount = sizeof(FilesystemList) / sizeof(FilesystemList[0]);

char** FindDisks(size_t* disksCount) {
	DIR* blockDirectory = opendir("/sys/block");
	if(!blockDirectory) {
		return NULL;
	}

	char** disks = NULL;
	size_t count = 0;
	struct dirent* entry;

	while((entry = readdir(blockDirectory)) != NULL) {
		if(entry->d_name[0] == '.') {
			continue;
		}

		char blockSysfsPath[PATH_MAX];
		snprintf(blockSysfsPath, sizeof(blockSysfsPath), "/sys/block/%s/device", entry->d_name);
		if(access(blockSysfsPath, F_OK) != 0) {
			continue;
		}

		char** grownDisks = realloc(disks, (count + 1) * sizeof(char*));
		if(!grownDisks) {
			for(size_t i = 0; i < count; i++) { free(disks[i]); }
			free(disks);
			closedir(blockDirectory);
			return NULL;
		}
		disks = grownDisks;

		char devicePath[PATH_MAX];
		snprintf(devicePath, sizeof(devicePath), "/dev/%s", entry->d_name);

		disks[count] = strdup(devicePath);
		if(!disks[count]) {
			for(size_t i = 0; i < count; i++) { free(disks[i]); }
			free(disks);
			closedir(blockDirectory);
			return NULL;
		}

		count++;
	}

	closedir(blockDirectory);

	*disksCount = count;
	return disks;
}

char* FindFilesystem(const char* devicePath) {
	char readFilesystemCommand[PATH_MAX];
	snprintf(readFilesystemCommand, sizeof(readFilesystemCommand), "blkid -o value -s TYPE \"%s\" 2>/dev/null", devicePath);

	FILE* commandPipe = popen(readFilesystemCommand, "r");
	if(!commandPipe) {
		return NULL;
	}

	char filesystemBuffer[128] = { 0 };
	if(!fgets(filesystemBuffer, sizeof(filesystemBuffer), commandPipe)) {
		pclose(commandPipe);
		return NULL;
	}
	pclose(commandPipe);

	filesystemBuffer[strcspn(filesystemBuffer, "\r\n")] = '\0';
	if(filesystemBuffer[0] == '\0') {
		return NULL;
	}

	return strdup(filesystemBuffer);
}

static bool PathsPointToSameDevice(const char* firstPath, const char* secondPath) {
	struct stat firstStat;
	struct stat secondStat;

	if(stat(firstPath, &firstStat) != 0) {
		return false;
	}

	if(stat(secondPath, &secondStat) != 0) {
		return false;
	}

	return firstStat.st_dev == secondStat.st_dev && firstStat.st_rdev == secondStat.st_rdev;
}

static char* FindDeviceAlias(const char* aliasDirectory, const char* partitionPath) {
	DIR* aliasDir = opendir(aliasDirectory);
	if(!aliasDir) {
		return NULL;
	}

	char* foundAlias = NULL;
	struct dirent* entry;

	while((entry = readdir(aliasDir)) != NULL) {
		if(entry->d_name[0] == '.') {
			continue;
		}

		char aliasLinkPath[PATH_MAX];
		snprintf(aliasLinkPath, sizeof(aliasLinkPath), "%s/%s", aliasDirectory, entry->d_name);

		if(PathsPointToSameDevice(aliasLinkPath, partitionPath)) {
			foundAlias = strdup(entry->d_name);
			break;
		}
	}

	closedir(aliasDir);
	return foundAlias;
}

void ReadPartitionSize(Partition* partition) {
	if(!partition || !partition->name) {
		return;
	}

	char partitionSizePath[PATH_MAX];
	snprintf(partitionSizePath, sizeof(partitionSizePath), "/sys/class/block/%s/size", partition->name);

	FILE* sizeFile = fopen(partitionSizePath, "r");
	if(!sizeFile) {
		return;
	}

	unsigned long long sectorCount = 0;
	if(fscanf(sizeFile, "%llu", &sectorCount) != 1) {
		fclose(sizeFile);
		return;
	}
	fclose(sizeFile);

	unsigned long long byteCount = sectorCount * 512ULL;

	char sizeBuffer[32];
	if(byteCount >= 1024ULL * 1024ULL * 1024ULL * 1024ULL) {
		snprintf(sizeBuffer, sizeof(sizeBuffer), "%.1fT", (double)byteCount / (1024.0 * 1024.0 * 1024.0 * 1024.0));
	}
	else if(byteCount >= 1024ULL * 1024ULL * 1024ULL) {
		snprintf(sizeBuffer, sizeof(sizeBuffer), "%.1fG", (double)byteCount / (1024.0 * 1024.0 * 1024.0));
	}
	else if(byteCount >= 1024ULL * 1024ULL) {
		snprintf(sizeBuffer, sizeof(sizeBuffer), "%.1fM", (double)byteCount / (1024.0 * 1024.0));
	}
	else if(byteCount >= 1024ULL) {
		snprintf(sizeBuffer, sizeof(sizeBuffer), "%.1fK", (double)byteCount / 1024.0);
	}
	else {
		snprintf(sizeBuffer, sizeof(sizeBuffer), "%llu", byteCount);
	}

	partition->sizeStr = strdup(sizeBuffer);
}

Partition* FindPartitions(const char* disk, size_t* partitionCount) {
	*partitionCount = 0;

	const char* diskName = strncmp(disk, "/dev/", 5) == 0 ? disk + 5 : disk;

	char partitionSysfsPath[PATH_MAX];
	snprintf(partitionSysfsPath, sizeof(partitionSysfsPath), "/sys/class/block/%s", diskName);

	DIR* blockDirectory = opendir(partitionSysfsPath);
	if(!blockDirectory) {
		return NULL;
	}

	Partition* partitions = NULL;
	struct dirent* entry;

	while((entry = readdir(blockDirectory)) != NULL) {
		if(entry->d_name[0] == '.') {
			continue;
		}

		char partitionMarkerPath[PATH_MAX];
		snprintf(partitionMarkerPath, sizeof(partitionMarkerPath), "%s/%s/partition", partitionSysfsPath, entry->d_name);

		if(access(partitionMarkerPath, F_OK) != 0) {
			continue;
		}

		Partition* grownPartitions = realloc(partitions, (*partitionCount + 1) * sizeof(*partitions));
		if(!grownPartitions) {
			break;
		}
		partitions = grownPartitions;

		Partition* partition = &partitions[*partitionCount];
		memset(partition, 0, sizeof(*partition));

		partition->name = strdup(entry->d_name);

		char devicePath[PATH_MAX];
		snprintf(devicePath, sizeof(devicePath), "/dev/%s", entry->d_name);
		partition->path = strdup(devicePath);

		ReadPartitionSize(partition);
		partition->filesystem = FindFilesystem(partition->path);

		partition->uuid = FindDeviceAlias("/dev/disk/by-uuid", partition->path);
		partition->label = FindDeviceAlias("/dev/disk/by-label", partition->path);

		(*partitionCount)++;
	}

	closedir(blockDirectory);
	return partitions;
}

static void RestoreSavedPartitionConfiguration(Partition* partitions, size_t partitionCount) {
	if(partitions == NULL || DATA.Partitioning.partitionsCount == 0) {
		return;
	}

	for(size_t i = 0; i < partitionCount; i++) {
		if(!partitions[i].path) {
			continue;
		}

		for(size_t j = 0; j < DATA.Partitioning.partitionsCount; j++) {
			const Partition* savedPartition = &DATA.Partitioning.partitions[j];
			if(!savedPartition->path) {
				continue;
			}

			if(strcmp(savedPartition->path, partitions[i].path) != 0) {
				continue;
			}

			if(savedPartition->mountPoint) {
				if(partitions[i].mountPoint) { free(partitions[i].mountPoint); }
				partitions[i].mountPoint = strdup(savedPartition->mountPoint);
			}
			if(savedPartition->filesystem) {
				if(partitions[i].filesystem) { free(partitions[i].filesystem); }
				partitions[i].filesystem = strdup(savedPartition->filesystem);
			}

			partitions[i].format = savedPartition->format;
			partitions[i].luks = savedPartition->luks;
			partitions[i].modified = true;
			break;
		}
	}
}

void FreePartitionContent(Partition* partition) {
	if(!partition) {
		return;
	}

	if(partition->mountPoint) { free(partition->mountPoint); }
	if(partition->label) { free(partition->label); }
	if(partition->uuid) { free(partition->uuid); }
	if(partition->name) { free(partition->name); }
	if(partition->path) { free(partition->path); }
	if(partition->filesystem) { free(partition->filesystem); }
	if(partition->sizeStr) { free(partition->sizeStr); }

	memset(partition, 0, sizeof(*partition));
}

static void FreeScreenData(char** disks, size_t disksCount, Partition* partitions, size_t partitionCount, char* selectedDisk) {
	for(size_t i = 0; i < disksCount; i++) {
		if(disks[i]) { free(disks[i]); }
	}
	if(disks) { free(disks); }
	disks = NULL;

	for(size_t i = 0; i < partitionCount; i++) {
		FreePartitionContent(&partitions[i]);
	}
	if(partitions) { free(partitions); }
	partitions = NULL;

	if(selectedDisk) { free(selectedDisk); }
	selectedDisk = NULL;
}

void CommitPartitions(const Partition* partitions, size_t partitionCount, const char* disk) {
	for(size_t i = 0; i < DATA.Partitioning.partitionsCount; i++) {
		FreePartitionContent(&DATA.Partitioning.partitions[i]);
	}
	if(DATA.Partitioning.partitions) {
		free(DATA.Partitioning.partitions);
		DATA.Partitioning.partitions = NULL;
	}
	if(DATA.Partitioning.selectedDisk) {
		free(DATA.Partitioning.selectedDisk);
	}

	DATA.Partitioning.partitionsCount = 0;
	DATA.Partitioning.selectedDisk = disk ? strdup(disk) : NULL;

	size_t storedCount = 0;
	for(size_t i = 0; i < partitionCount; i++) {
		if(!partitions[i].modified) {
			continue;
		}

		Partition* grownPartitions = realloc(DATA.Partitioning.partitions, (storedCount + 1) * sizeof(*grownPartitions));
		if(!grownPartitions) {
			break;
		}
		DATA.Partitioning.partitions = grownPartitions;

		Partition* storedPartition = &DATA.Partitioning.partitions[storedCount];
		memset(storedPartition, 0, sizeof(*storedPartition));

		const Partition* sourcePartition = &partitions[i];
		if(sourcePartition->mountPoint) { storedPartition->mountPoint = strdup(sourcePartition->mountPoint); }
		if(sourcePartition->label) { storedPartition->label = strdup(sourcePartition->label); }
		if(sourcePartition->uuid) { storedPartition->uuid = strdup(sourcePartition->uuid); }
		if(sourcePartition->name) { storedPartition->name = strdup(sourcePartition->name); }
		if(sourcePartition->path) { storedPartition->path = strdup(sourcePartition->path); }
		if(sourcePartition->filesystem) { storedPartition->filesystem = strdup(sourcePartition->filesystem); }
		if(sourcePartition->sizeStr) { storedPartition->sizeStr = strdup(sourcePartition->sizeStr); }

		storedPartition->format = sourcePartition->format;
		storedPartition->luks = sourcePartition->luks;
		storedPartition->modified = true;

		storedCount++;
	}

	DATA.Partitioning.partitionsCount = storedCount;
}

void ScreenPartitions(void) {
	static bool inited = false;

	static char** disks = NULL;
	static size_t disksCount = 0;

	static PartitionScreenPhase phase = PARTITION_SCREEN_SELECT_DISK;
	static char* selectedDisk = NULL;
	static Partition* partitions = NULL;
	static size_t partitionsCount = 0;
	static int listOffset = 0;
	static size_t editIndex = 0;

	if(!inited) {
		disks = FindDisks(&disksCount);

		phase = PARTITION_SCREEN_SELECT_DISK;
		selectedDisk = NULL;
		partitions = NULL;
		partitionsCount = 0;
		listOffset = 0;
		editIndex = 0;

		inited = true;
	}

	if(phase == PARTITION_SCREEN_SELECT_DISK) {
		char* pickedDiskName = NULL;
		bool wasSelected = false;
		bool wasCustomInput = false;
		size_t pickedDiskNameSize = 0;

		SelectFromList("Select disk", "Custom: ", (const char**)disks, disksCount, &pickedDiskName, &wasSelected, &pickedDiskNameSize, &wasCustomInput);

		if(!wasSelected || !pickedDiskName) {
			return;
		}

		selectedDisk = strdup(pickedDiskName);
		if(wasCustomInput) {
			free(pickedDiskName);
		}

		EndFrame();
		CloseTui();

		pid_t childPid = fork();

		if(childPid == 0) {
			char* arguments[] = {
				"cfdisk",
				selectedDisk,
				NULL
			};

			execvp("cfdisk", arguments);

			exit(EXIT_FAILURE);
		}
		else if(childPid != -1) {
			int childStatus = 0;
			while(waitpid(childPid, &childStatus, 0) == -1) {
				if(errno != EINTR) {
					break;
				}
			}
		}

		InitTui(20, TUI_DYNAMIC);
		BeginFrame();

		ESleep(0, 250, 0);

		partitions = FindPartitions(selectedDisk, &partitionsCount);
		RestoreSavedPartitionConfiguration(partitions, partitionsCount);

		phase = PARTITION_SCREEN_LIST_PARTITIONS;
		DATA.cursorPos = 3;
		listOffset = 0;

		DATA.screenState = SCREEN_PARTITIONS;
		return;
	}

	if(phase == PARTITION_SCREEN_LIST_PARTITIONS) {
		ClearTui(TERMBLACK, TERMWHITE);

		DrawTextf("Partitions on %s", 0, 0, TERMWHITE, selectedDisk ? selectedDisk : "?");

		int visibleRows = GetLastTuiIndex().y - 4;
		if(visibleRows < 1) {
			visibleRows = 1;
		}

		int totalItems = (int)partitionsCount + 1;
		int shownItems = totalItems > visibleRows ? visibleRows : totalItems;

		int maximumOffset = totalItems - visibleRows;
		if(maximumOffset < 0) {
			maximumOffset = 0;
		}

		int maximumCursorPosition = 3 + shownItems - 1;

		char cursorMoveDirection = HandleCursor(3, maximumCursorPosition, NULL, 0);

		if(cursorMoveDirection == 1) {
			if(listOffset > 0) { listOffset--; }
		}
		else if(cursorMoveDirection == -1) {
			if(listOffset < maximumOffset) { listOffset++; }
		}

		for(int i = 0; i < shownItems; i++) {
			int itemIndex = listOffset + i;

			if(itemIndex == (int)partitionsCount) {
				DrawText("Done", 4, 3 + i, TERMWHITE);
				continue;
			}

			Partition* partition = &partitions[itemIndex];

			char rowBuffer[PATH_MAX];
			snprintf(rowBuffer, sizeof(rowBuffer), "%s  %s  %s  %s%s%s",
				partition->path ? partition->path : "?",
				partition->sizeStr ? partition->sizeStr : "?",
				partition->filesystem ? partition->filesystem : "none",
				(partition->mountPoint && partition->mountPoint[0]) ? partition->mountPoint : "(none)",
				partition->luks ? "  [LUKS]" : "",
				partition->format ? "  [FORMAT]" : "");

			DrawText(rowBuffer, 4, 3 + i, TERMWHITE);
		}

		DrawText("-------------------------", 4, 3 + shownItems + 1, TERMWHITE);

		if(IsKeyPressed(KEY_ENTER)) {
			int selectedIndex = listOffset + (DATA.cursorPos - 3);

			if(selectedIndex == (int)partitionsCount) {
				CommitPartitions(partitions, partitionsCount, selectedDisk);

				FreeScreenData(disks, disksCount, partitions, partitionsCount, selectedDisk);
				disks = NULL;
				disksCount = 0;
				partitions = NULL;
				partitionsCount = 0;
				selectedDisk = NULL;

				inited = false;
				DATA.screenState = SCREEN_MAIN;
			}
			else if(selectedIndex >= 0 && selectedIndex < (int)partitionsCount) {
				editIndex = (size_t)selectedIndex;
				phase = PARTITION_SCREEN_EDIT_PARTITION;
			}
		}
		else if(IsKeyPressed(KEY_ESCAPE)) {
			FreeScreenData(disks, disksCount, partitions, partitionsCount, selectedDisk);
			disks = NULL;
			disksCount = 0;
			partitions = NULL;
			partitionsCount = 0;
			selectedDisk = NULL;

			inited = false;
			DATA.screenState = SCREEN_MAIN;
		}

		return;
	}

	if(phase == PARTITION_SCREEN_EDIT_PARTITION) {
		ClearTui(TERMBLACK, TERMWHITE);

		Partition* partition = &partitions[editIndex];

		DrawTextf("Partition: %s", 0, 0, TERMWHITE, partition->path ? partition->path : "?");

		DrawTextf("   Mount point:      %s", 3, 2, TERMWHITE, (partition->mountPoint && partition->mountPoint[0]) ? partition->mountPoint : "None");
		DrawTextf("   Filesystem:       %s", 3, 3, TERMWHITE, partition->filesystem ? partition->filesystem : "None");
		DrawTextf("   Format:           %s", 3, 4, TERMWHITE, partition->format ? "yes" : "no");
		DrawTextf("   LUKS:             %s", 3, 5, TERMWHITE, partition->luks ? "yes" : "no");

		DrawText("Save", 3, 7, TERMWHITE);

		int skippedCursorRows[1] = { 6 };
		HandleCursor(2, 7, skippedCursorRows, 1);

		if(IsKeyPressed(KEY_ESCAPE)) {
			phase = PARTITION_SCREEN_LIST_PARTITIONS;
			DATA.cursorPos = 3;
		}

		if(IsKeyPressed(KEY_ENTER)) {
			switch(DATA.cursorPos) {
				case 2: {
					DrawLineEx(" ", 24, 2, GetLastTuiIndex().x, 2, &TERMWHITE, &TERMBLACK, 1);

					char* inputString = GetStringFromUser((Vector2i){ 24, DATA.cursorPos });
					if(!inputString) {
						return;
					}

					if(inputString[0] == '\0' || inputString[0] == '\n') {
						free(inputString);
					}
					else {
						if(!partition->mountPoint || strcmp(partition->mountPoint, inputString) != 0) {
							if(partition->mountPoint) { free(partition->mountPoint); }
							partition->mountPoint = inputString;
							partition->modified = true;
						}
						else {
							free(inputString);
						}
					}
					break;
				}
				case 3: {
					size_t filesystemIndex = 0;

					if(partition->filesystem) {
						for(size_t i = 0; i < FilesystemCount; i++) {
							if(strcmp(partition->filesystem, FilesystemList[i]) == 0) {
								filesystemIndex = i;
								break;
							}
						}
					}

					filesystemIndex++;
					if(filesystemIndex >= FilesystemCount) {
						filesystemIndex = 0;
					}

					if(partition->filesystem) { free(partition->filesystem); }
					partition->filesystem = strdup(FilesystemList[filesystemIndex]);
					partition->modified = true;
					break;
				}
				case 4:
					partition->format = !partition->format;
					partition->modified = true;
					break;
				case 5:
					partition->luks = !partition->luks;
					partition->modified = true;
					break;
				case 7:
					phase = PARTITION_SCREEN_LIST_PARTITIONS;
					DATA.cursorPos = 3;
					break;
			}
		}

		return;
	}
}
