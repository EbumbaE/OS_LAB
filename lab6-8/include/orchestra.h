#pragma once

#include <io.h>
#include <fcntl.h>
#include <process.h>
#include <math.h>
#include <tchar.h>
#include <crtdbg.h>
#include <sys/types.h>
#include <conio.h>
#include <malloc.h>
#include <windows.h>

#include "../include/tree.h"

typedef struct _parent {
    Node *root;
    int id;
    struct _parent* next;
    struct _parent* prev;
} Parent;

typedef struct _conductor {
    Parent* begin;
    Parent* end;
    int size;
} Conductor;

Conductor* NewConductor();
void DeleteConductor(Conductor* conductor);

int AddParent(Conductor* conductor, int id);
int DeleteParent(Conductor* conductor, int id);
int AmountParents(Conductor* conductor);

int AddChild(Conductor* conductor, int parentID, int childID);
int DeleteChild(Conductor* conductor, int parentID, int childID);

int StartTimer(Conductor* conductor, int childID);
int StopTimer(Conductor* conductor, int childID);
int GetTime(Conductor* conductor, int childID);
int ExecChild(char* command, int childID);