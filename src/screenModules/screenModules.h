#ifndef FINIX_INSTALLER_SCREEN_MODULES_H
#define FINIX_INSTALLER_SCREEN_MODULES_H

#include "../../libs/esclib.h"

#include <stddef.h>
#include <stdbool.h>

char HandleCursor(int lowestPos, int maxPos, const int* skip, size_t skipSize);

void SelectFromConstList(const char* title, const char* const* list, size_t sizeOfList, char** dumpSelectedTo, bool* dumpIsSelected);
void SelectFromConstListOrCustom(const char* title, const char* customPrompt, const char* const* list, size_t sizeOfList, char** dumpSelectedTo, bool* dumpIsSelected, size_t* dumpSizeOfTo, bool* dumpIsCustomTo);

char* GetStringFromUser(Vector2i currPos);

#endif
