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
#include <malloc.h>
#include <assert.h>

#include <stdio.h>
#include <stdlib.h>

#define ADDRESS_A_C "tcp://127.0.0.1:5555"
#define ADDRESS_A_B "tcp://127.0.0.1:5556"
#define ADDRESS_B_C "tcp://127.0.0.1:5557"

#include "zmq.h"