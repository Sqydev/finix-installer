#ifndef FINIX_INSTALLER_FNS_H
#define FINIX_INSTALLER_FNS_H

#include <stddef.h>

void screenmain(void);
void DoScreenmain(void);
void HandleCursor(int lowestPos, int maxPos, const int* skip, size_t skipSize);
void SelectFromList(const char* const* list, size_t size, size_t* dumpSelectedTo, bool* dumpIsSelected);

char** ExtractFile(char* path, size_t* dumpLinesCount);
char** ExtractDirContents(char* path, size_t* dumpContentCount);

void CleanUp(void);

const char* const* GetTimeZones(size_t* dumpSizeTo);

#endif
