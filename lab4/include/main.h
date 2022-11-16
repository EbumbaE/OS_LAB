#pragma once
#include "include.h"

HANDLE createPublicEvent(LPTSTR name);
void checkNULLEvent(HANDLE event);
int CreateChildProcess(TCHAR *childName);