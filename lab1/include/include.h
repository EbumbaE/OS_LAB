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

#include "stdlib.h"
#include "stdio.h"

int const INPUT_READ = 0, INPUT_WRITE = 1, 
          OUTPUT_READ = 2, OUTPUT_WRITE = 3;