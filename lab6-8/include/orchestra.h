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
#include "../include/msg_q.h"

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
int CountTrace(Conductor *conductor, int *trace, int childID);

int AddParent(Conductor* conductor, int id);
int DeleteParent(Conductor* conductor, int id);
int AmountParents(Conductor* conductor);

int AddChild(Conductor* conductor, int parentID, int childID);
int DeleteChild(Conductor* conductor, int parentID, int childID);

int PingNode(Conductor* conductor, int id);

#define ErrorNotFoundParent         1
#define ErrorParentAlreadyExist     2
#define ErrorNotFoundChild          3
#define ErrorChildAlreadyExist      4
#define ErrorInCreateChildProccess  5
#define ErrorInCreatePipe           6
#define ErrorNotFoundNode           7