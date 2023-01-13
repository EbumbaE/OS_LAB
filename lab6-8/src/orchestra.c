#include "../include/orchestra.h"

Conductor* NewConductor(){
    Conductor* conductor = (Conductor*)malloc(sizeof(conductor));
    conductor->size = 0;
    conductor->begin = NULL;

    return conductor;
}

void DeleteConductor(Conductor* conductor) {
    Parent* p;
    while (conductor->begin != NULL) {
        p = conductor->begin;
        conductor->begin = conductor->begin->next;
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
    
    int err = CreateChildProcess("parent", id, pid);
    if (!err){
        return ErrorInCreateChildProccess;
    }
    return 0;
}

int DeleteParent(Conductor* conductor, int id) {
    if (conductor->begin == NULL) {
        return 0;
    } 

    if (conductor->size == 1 && conductor->begin->id == id) {
        deleteTree(conductor->begin->root);
        free(conductor->begin);
        conductor = NewConductor();
        return 0;
    }

    Parent* pIter = conductor->begin;
    while (pIter != NULL && pIter->id != id) {
        pIter = pIter->next;
    }

    if (pIter == NULL) {
        return ErrorNotFoundParent;
    }

    while (pIter != NULL) {
        Parent *prev = pIter->prev, *next = pIter->next;
        if (pIter->id == id) {
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
            deleteTree(pIter->root);
            free(pIter);
            break;
        }
        pIter = next;
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
    while (pIter != NULL) {
        if (pIter->id == parentID) {
            if (nodeExist(pIter->root, childID)) {
                return ErrorChildAlreadyExist;
            }

            int err = CreateChildProcess("child", childID, pid);
            if (!err){
                return ErrorInCreateChildProccess;
            }

            pIter->root = insertNode(pIter->root, childID);

            break;
        } 
        pIter = pIter->next;
    }

    if (pIter == NULL) {
        return ErrorNotFoundParent;
    }

    return 0;
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
            break;
        } 
        pIter = pIter->next;
    }

    if (pIter == NULL) {
        return ErrorNotFoundParent;
    }

    return 0;
}

int PingNode(Conductor* conductor, int id) {
    if (conductor->begin == NULL) {
        return 0;
    }

    Parent *pIter = conductor->begin;
    while (pIter != NULL) {
        if (pIter->id == id || nodeExist(pIter->root, id)) {
            return pingProcess(id + MIN_ADDR);
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
            trace[0] = pIter->id;
            countTrace(pIter->root, childID, trace, 1, 0);
            return 0;
        }
    }
    return ErrorNotFoundNode;
}

int CreateChildProcess(char *childName, int id, int *pid){
    char argID[MN];
    sprintf(argID, "%d", id);
    char *args[] = {childName, argID, NULL}; 
    *pid = fork();
    if (pid == -1) {
        return ErrorInCreateChildProccess;
    }
    if (pid == 0) {
        execv(childName, args);
        return 0;
    }
    return -1;
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
    
    void *context = createZmqContext();
    void *responder = createZmqSocket(context, ZMQ_REP);
    void *requester = createZmqSocket(context, ZMQ_REQ);
    
    char orchestraAddr[30] = SERVER_SOCKET_PATTERN;
    createAddr(&orchestraAddr, orchestraID + MIN_ADDR);
    bindZmqSocket(responder, orchestraAddr);

    char parentAddr[30] = SERVER_SOCKET_PATTERN;    

    message msg = {cmd: -1};
    char *command;
    int parentID, childID, t;
    int err;
    while (msg.cmd != CMD_EXIT) {
        receiveMessage(responder, &msg);

        switch (msg.cmd) {
        case CREATE_CHILD:
            if (msg.childID < 1) {
                msg.error = ErrorChildAlreadyExist;
                sendMessage(responder, &msg);
                break;
            }

            err = AddChild(conductor, msg.parentID, msg.childID, &msg.pid);
            if (err != 0) {
                msg.error = err;
                sendMessage(responder, &msg);
                break;
            }
            
            err = CountTrace(conductor, &(msg.trace), childID);
            if (err != 0) {
                msg.error = err;
                sendMessage(responder, &msg);
                break;
            }

            reconnectZmqSocket(requester, parentID + MIN_ADDR, parentAddr);
            sendMessage(requester, &msg);
            receiveMessage(requester, &msg);
            
            sendMessage(responder, &msg);
            break;

        case CREATE_PARENT:
            if (msg.parentID < 1) {
                msg.error = ErrorParentAlreadyExist;
                sendMessage(responder, &msg);
                break;
            }

            msg.error = AddParent(conductor, parentID, &msg.pid);

            sendMessage(responder, &msg);
            break;
        
        case DELETE_CHILD:
            err = DeleteChild(conductor, parentID, childID);
            if (err != 0) {
                msg.error = err;
                sendMessage(responder, &msg);
                break;
            }

            err = CountTrace(conductor, &(msg.trace), childID);
            if (err != 0) {
                msg.error = err;
                sendMessage(responder, &msg);
                break;
            }
            reconnectZmqSocket(requester, parentID + MIN_ADDR, parentAddr);
            sendMessage(requester, &msg);
            receiveMessage(requester, &msg);
            
            sendMessage(responder, &msg);
            break;

        case DELETE_PARENT:
            err = DeleteParent(conductor, parentID);                
            if (err != 0) {
                msg.error = err;
                sendMessage(responder, &msg);
                break;
            }
            sendMessage(responder, &msg);
            break;

        case PING_NODE:
            msg.error = PingNode(conductor, msg.pid);
            sendMessage(responder, &msg);
            break;

        case CMD_START || CMD_STOP || CMD_TIME:
            err = CountTrace(conductor, &(msg.trace), childID);
            if (err != 0) {
                msg.error = err;
                receiveMessage(responder, &msg);
                break;
            }

            reconnectZmqSocket(requester, parentID + MIN_ADDR, parentAddr);
            sendMessage(requester, &msg);
            receiveMessage(requester, &msg);
            
            sendMessage(responder, &msg);
        }
    }

    DeleteConductor(conductor);
}