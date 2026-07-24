#ifndef FINIX_INSTALLER_SCREEN_MODULES_H
#define FINIX_INSTALLER_SCREEN_MODULES_H

#include <stddef.h>
#include <stdbool.h>

char HandleCursor(int lowestPos, int maxPos, const int* skip, size_t skipSize);

void SelectFromConstList(const char* title, const char* const* list, size_t sizeOfList, char** dumpSelectedTo, bool* dumpIsSelected);

void GetStringFromUser(const char* title, char** dumpStringTo, bool* dumpIsStringDone);

#endif
