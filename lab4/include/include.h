#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define _CRTDBG_MAP_ALLOC

#include <io.h>
#include <fcntl.h>
#include <process.h>
#include <math.h>
#include <windows.h>
#include <tchar.h>
#include <crtdbg.h>
#include <sys/types.h>
#include <conio.h>

#include "stdlib.h"
#include "stdio.h"

#define BUF_SIZE 256
TCHAR fileMappingName[]=TEXT("FileMappingObject");
TCHAR nameWaitChildEndEvent[]=TEXT("WaitWorkChild");
TCHAR nameRunChild1Event[]=TEXT("RunChild1");
TCHAR nameRunChild2Event[]=TEXT("RunChild2");
TCHAR nameChild1TerminateEvent[]=TEXT("TerminateChild1");
TCHAR nameChild2TerminateEvent[]=TEXT("TerminateChild2");