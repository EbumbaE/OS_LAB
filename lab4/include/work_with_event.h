#pragma once

#include <windows.h>
#include <tchar.h>
#include <crtdbg.h>
#include <sys/types.h>
#include <conio.h>

HANDLE openPublicEvent(LPTSTR name);
HANDLE createPublicEvent(LPTSTR name);
void checkNULLEvent(HANDLE event);
void trySignalToEvent(HANDLE event);