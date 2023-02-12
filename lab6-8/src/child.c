#include "../include/child.h"
#include "../include/msg_q.h"

int main(int argc, char const *argv[]) {

    if (argc < 3) {
        printf("error: amount argv\n");
        return 1;
    }
    
    int id, isParent;
    id = atoi(argv[1]);
    isParent = atoi(argv[2]);

    printf("[%d] has been created\n", getpid());

    void *context = createZmqContext();
    void *responder = createZmqSocket(context, ZMQ_REP);
    void *requester = createZmqSocket(context, ZMQ_REQ);
    char childAddr[MN] = SERVER_SOCKET_PATTERN;
    char addr[MN] = SERVER_SOCKET_PATTERN;
    
    createAddr(&childAddr, id + MIN_ADDR);
    bindZmqSocket(responder, childAddr);
    
    clock_t start = -1, stop = -1, timer = 0;

    message msg;
    
    while(1) {
        receiveMessage(responder, &msg);
    
        if (msg.trace[0] != 0) {
            int to = msg.trace[0];
            
            int i = 0;
            while(msg.trace[i] != 0) {
                msg.trace[i] = msg.trace[i + 1];
                i++;
            }
            
            reconnectZmqSocket(requester, to + MIN_ADDR, addr);
            sendMessage(requester, &msg);
            receiveMessage(requester, &msg);
            
            sendMessage(responder, &msg);
            continue;
        }

        if (msg.cmd == CHANGE_ROLE) {
            isParent = 1 - isParent;
            msg.error = 0;
            msg.pid = getpid();
            sendMessage(responder, &msg);
            continue;
        }

        if (msg.cmd == DELETE_CHILD || msg.cmd == DELETE_PARENT) {
            msg.error = 0;
            msg.pid = getpid();
            sendMessage(responder, &msg);
            break;
        }

        if (msg.cmd == CMD_START && !isParent) {
            start = clock();
            stop = -1;
            msg.pid = getpid();
            sendMessage(responder, &msg);
            continue;
        }
        
        if (msg.cmd == CMD_STOP && !isParent) {
            stop = clock();
            if (start != -1 && stop != -1) { 
                timer += stop - start;
            }
            start = -1;
            stop = -1;
            msg.pid = getpid();
            sendMessage(responder, &msg);
            continue;
        }

        if (msg.cmd == PING_NODE) {
            msg.error = 0;
            msg.pid = getpid();
            sendMessage(responder, &msg);
            continue;
        }

        if (msg.cmd == CMD_TIME) {
            msg.time = 0;
            if (!isParent) {
                if (start != -1 && stop == -1) {
                    msg.time =  timer + clock() - start;
                } else {
                    msg.time = timer;
                }
                msg.pid = getpid();
            }
            sendMessage(responder, &msg);
            continue;
        }
        msg.error = 10001;
        sendMessage(responder, &msg);
    }

    closeZmqSocket(responder);
    if (isParent) {
        printf("parent[%d]: by by\n", getpid());
    } else {
        printf("child[%d]: by by\n", getpid());
    }
    
    return 0;
}