#include "../include/orchestra.h"

Conductor* NewConductor(){
    Conductor* conductor = (Conductor*)malloc(sizeof(conductor));
    conductor->size = 0;
    conductor->begin = NULL;

    void *context = createZmqContext();
    conductor->responder = createZmqSocket(context, ZMQ_REP);
    conductor->requester = createZmqSocket(context, ZMQ_REQ);

    memset(conductor->addr, '\0', MN);
    strcat(conductor->addr, SERVER_SOCKET_PATTERN);

    return conductor;
}

int recDeleteChildProcess(Conductor *conductor, Node *root, int *trace, int n) {
    if (root == NULL) {
        return 0;
    }
    trace[n] = root->id;
    
    recDeleteChildProcess(conductor, root->left, trace, n + 1);
    recDeleteChildProcess(conductor, root->right, trace, n + 1);

    message msg;
    msg.cmd = DELETE_CHILD;
    memcpy(msg.trace, trace, 100);
    
    int toID = popFirstID(&(msg.trace));

    reconnectZmqSocket(conductor->requester, toID + MIN_ADDR, conductor->addr);
    sendMessage(conductor->requester, &msg);
    receiveMessage(conductor->requester, &msg);
    
    trace[n] = 0;
    return 0;
}

int DeleteChildProcesses(Conductor *conductor, Node *root) {
    int trace[100];
    memset(trace, 0, 100);
    recDeleteChildProcess(conductor, root, &trace, 0);
    return 0;
}

void DeleteConductor(Conductor* conductor) {
    Parent* p;
    while (conductor->begin != NULL) {
        p = conductor->begin;
        conductor->begin = conductor->begin->next;
        DeleteChildProcesses(conductor, p->root);
        deleteTree(p->root);
        free(p);
    }
    conductor->size = 0;
}

int checkExist(Conductor* conductor, int id) {
    Parent* p = conductor->begin;
    while (p != NULL) {
        if (p->id == id) {
            return 1;
        }
        p = p->next;
    }
    return 0;
}

int AddParent(Conductor* conductor, int id, int *pid) {
    if (conductor->begin == NULL) {
        Parent* parent = (Parent*)malloc(sizeof(Parent));
        parent->id = id;
        parent->next = NULL;
        parent->prev = NULL;
        parent->root = NULL;

        conductor->begin = parent;
        conductor->end = parent;
        conductor->size = 1;
    }
    else {
        if (checkExist(conductor, id)) {
            return ErrorParentAlreadyExist;
        }
        Parent* parent = (Parent*)malloc(sizeof(Parent));
        parent->id = id;
        parent->next = NULL;
        parent->prev = conductor->end;
        parent->root = NULL;

        conductor->end->next = parent;
        conductor->end = parent;
        conductor->size++;
    }

    conductor->end->root = insertNode(conductor->end->root, id);
    
    int err = CreateChildProcess("child", id, pid, 1);
    return err;
}

void ChangeRole(void *requester, char *addr, int toID) {
    message msg;
    memset(msg.trace, 0, 100);
    msg.cmd = CHANGE_ROLE;
    reconnectZmqSocket(requester, toID + MIN_ADDR, addr);
    sendMessage(requester, &msg);
    receiveMessage(requester, &msg);
}

int DeleteParent(Conductor* conductor, int id) {
    if (conductor->begin == NULL) {
        return ErrorNotFoundParent;
    } 

    Parent* pIter = conductor->begin;
    if (conductor->size == 1 && pIter->id == id) {
        DeleteChildProcesses(conductor, pIter->root);
        deleteTree(pIter->root);
        conductor->begin = conductor->end = NULL; 
        free(pIter);
        return 0;
    }

    while (pIter != NULL && pIter->id != id) {
        pIter = pIter->next;
    }

    if (pIter == NULL) {
        return ErrorNotFoundParent;
    }

    if (pIter != NULL) {
        DeleteChildProcesses(conductor, pIter->root);
        deleteTree(pIter->root);

        Parent *prev = pIter->prev, *next = pIter->next;
        if (prev != NULL) { 
            pIter->prev->next = next;
        } else {
            conductor->begin = next;
            conductor->begin->prev = NULL;
        }
        if (next != NULL) {
            pIter->next->prev = prev;
        } else {
            conductor->end = prev;
            conductor->end->next = NULL;
        }
        free(pIter);
    }
    return 0;
}

int AmountParents(Conductor* c) {
    return c->size;
}

int AddChild(Conductor* conductor, int parentID, int childID, int *pid) {
    if (conductor->begin == NULL) {
        return ErrorNotFoundParent;
    }

    Parent *pIter = conductor->begin;
    Parent *parentIter = NULL;
    while (pIter != NULL) {
        if (pIter->id == parentID) {
            parentIter = pIter;
        }
        if (pIter->id == childID) {
            return ErrorChildAlreadyExist;
        }
        if (nodeExist(pIter->root, childID)) {
            return ErrorChildAlreadyExist;
        } 
        pIter = pIter->next;
    }

    if (parentIter == NULL) {
        return ErrorNotFoundParent;
    }

    parentIter->root = insertNode(parentIter->root, childID);

    int err = CreateChildProcess("child", childID, pid, 0);
    if (err != 0) {
        return err;
    }

    if (parentIter->root->id != parentIter->id) {
        printf("new root is [%d]  past root: %d \n", parentIter->root->id, parentIter->id);
        message msg;
        int toParent = parentIter->root->id;
        int toChild =  parentIter->id;
        parentIter->id = parentIter->root->id;
        
        ChangeRole(conductor->requester, conductor->addr, toParent);
        ChangeRole(conductor->requester, conductor->addr, toChild);
    }

    return err;
}

int DeleteChild(Conductor* conductor, int parentID, int childID) {
    if (conductor->begin == NULL) {
        return ErrorNotFoundParent;
    }

    Parent *pIter = conductor->begin;
    while (pIter != NULL) {
        if (pIter->id == parentID) {
            if (!nodeExist(pIter->root, childID)) {
                return ErrorNotFoundChild;
            }
            pIter->root = deleteNode(pIter->root, childID);
            if (pIter->id != pIter->root->id) {
                printf("new root is [%d]  past root: %d \n", pIter->root->id, pIter->id);
                int toParent = pIter->id;
                int toChild = pIter->root->id;
                pIter->id = pIter->root->id;
                ChangeRole(conductor->requester, conductor->addr, toChild);
                if (toParent != parentID) {
                    ChangeRole(conductor->requester, conductor->addr, toParent);
                }
            }
            break;
        } 
        pIter = pIter->next;
    }

    if (pIter == NULL) {
        return ErrorNotFoundParent;
    }

    return 0;
}

int PingNode(Conductor* conductor, int id, int *pid) {
    if (conductor->begin == NULL) {
        return ErrorNotFoundNode;
    }

    Parent *pIter = conductor->begin;
    while (pIter != NULL) {
        if (pIter->id == id || nodeExist(pIter->root, id)) {
            if (pingProcess(id + MIN_ADDR, pid)) {
                return 0;
            } else {
                return 10001;
            }
        }
        pIter = pIter->next;
    }
    return ErrorNotFoundNode;
}

int CountTrace(Conductor *conductor, int *trace, int childID) {
    Parent *pIter = conductor->begin;
    while (pIter != NULL) {
        if (nodeExist(pIter->root, childID)) {
            memset(trace, 0, 100);
            countTrace(pIter->root, childID, trace);
            return 0;
        }
        pIter = pIter->next;
    }
    return ErrorNotFoundNode;
}

int CreateChildProcess(char *childName, int id, int *pid, int isParent){
    char argID[MN];
    sprintf(argID, "%d", id);
    char argIsParent[MN];
    sprintf(argIsParent, "%d", isParent);
    char *args[] = {childName, argID, argIsParent, NULL}; 
    *pid = fork();
    if (*pid == -1) {
        return ErrorInCreateChildProccess;
    }
    if (*pid == 0) {
        execv(childName, args);
    }
    return 0;
}

void PrintOrchestra(Conductor *conductor) {
    Parent *pIter = conductor->begin;
    while (pIter != NULL) {
        printf("tree for parent [%d]: \n", pIter->id);
        printTree(pIter->root);
        pIter = pIter->next;
    }
}

int popFirstID(int *trace) {
    int fID = trace[0];
    int i = 0;
    while (trace[i] != 0) {
        trace[i] = trace[i + 1];
        i++;
    }
    return fID;
}

int main(int argc, char const *argv[]) {

    if (argc < 2) {
        printf("error: amount argv\n");
        return 1;
    }
    
    int orchestraID = atoi(argv[1]);

    int pid = getpid();
    printf("orchestra [%d] has been created\n", pid);

    Conductor* conductor = NewConductor();
    
    char orchestraAddr[30] = SERVER_SOCKET_PATTERN;
    createAddr(&orchestraAddr, orchestraID + MIN_ADDR);
    bindZmqSocket(conductor->responder, orchestraAddr);

    message msg = {cmd: -1};
    char *command;
    int err, toParentID;
    while (msg.cmd != CMD_EXIT) {
        receiveMessage(conductor->responder, &msg);

        switch (msg.cmd) {
        case CREATE_CHILD:
            if (msg.childID < 1 || msg.parentID < 1) {
                msg.error = ErrorIncorrectData;
                sendMessage(conductor->responder, &msg);
                break;
            }

            err = AddChild(conductor, msg.parentID, msg.childID, &(msg.pid));
            if (err != 0) {
                msg.error = err;
                sendMessage(conductor->responder, &msg);
                break;
            }

            sendMessage(conductor->responder, &msg);
            break;

        case CREATE_PARENT:
            if (msg.parentID < 1) {
                msg.error = ErrorIncorrectData;
                sendMessage(conductor->responder, &msg);
                break;
            }

            msg.error = AddParent(conductor, msg.parentID, &msg.pid);

            sendMessage(conductor->responder, &msg);
            break;
        
        case DELETE_CHILD:
            err = CountTrace(conductor, &(msg.trace), msg.childID);
            if (err != 0) {
                msg.error = err;
                sendMessage(conductor->responder, &msg);
                break;
            }

            toParentID = popFirstID(&(msg.trace));

            reconnectZmqSocket(conductor->requester, toParentID + MIN_ADDR, conductor->addr);
            sendMessage(conductor->requester, &msg);
            receiveMessage(conductor->requester, &msg);
            
            msg.error = DeleteChild(conductor, msg.parentID, msg.childID);
            sendMessage(conductor->responder, &msg);
            break;

        case DELETE_PARENT:
            err = DeleteParent(conductor, msg.parentID);                
            if (err != 0) {
                msg.error = err;
                sendMessage(conductor->responder, &msg);
                break;
            }
            sendMessage(conductor->responder, &msg);
            break;

        case PING_NODE:
            if (msg.pid < 0) {
                msg.error = ErrorIncorrectData;
                sendMessage(conductor->responder, &msg);
                break;
            }
            if (msg.pid == 0) {
                msg.error = 0;
                msg.pid = getpid();
                sendMessage(conductor->responder, &msg);
                break;
            }
            msg.error = PingNode(conductor, msg.pid, &(msg.pid));
            sendMessage(conductor->responder, &msg);
            break;

        case CMD_START: 
        case CMD_STOP: 
        case CMD_TIME:
            err = CountTrace(conductor, &(msg.trace), msg.childID);
            if (err != 0) {
                msg.error = err;
                sendMessage(conductor->responder, &msg);
                break;
            }

            toParentID = popFirstID(&(msg.trace));

            reconnectZmqSocket(conductor->requester, toParentID + MIN_ADDR, conductor->addr);
            sendMessage(conductor->requester, &msg);
            receiveMessage(conductor->requester, &msg);
            
            sendMessage(conductor->responder, &msg);
            break;

        case PRINT_ORCHESTRA:
            PrintOrchestra(conductor);
            msg.error = 0;
            sendMessage(conductor->responder, &msg);
            break;
        }
    }

    DeleteConductor(conductor);
}