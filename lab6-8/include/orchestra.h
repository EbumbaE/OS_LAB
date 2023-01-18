#pragma once

#include <sys/types.h>
#include <malloc.h>

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

    void *responder;
    void *requester;
    char addr[MN];
} Conductor;

Conductor* NewConductor();
void DeleteConductor(Conductor* conductor);
int CountTrace(Conductor *conductor, int *trace, int childID);

int AddParent(Conductor* conductor, int id, int *pid);
int DeleteParent(Conductor* conductor, int id);
int AmountParents(Conductor* conductor);

int AddChild(Conductor* conductor, int parentID, int childID, int *pid);
int DeleteChild(Conductor* conductor, int parentID, int childID);

int PingNode(Conductor* conductor, int id, int *pid);

void PrintOrchestra(Conductor *conductor);

#define ErrorNotFoundParent         1
#define ErrorParentAlreadyExist     2
#define ErrorNotFoundChild          3
#define ErrorChildAlreadyExist      4
#define ErrorInCreateChildProccess  5
#define ErrorNotFoundNode           6
#define ErrorIncorrectData          7
#define ErrorNotDone                8