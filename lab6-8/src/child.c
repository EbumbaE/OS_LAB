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
    char childAddr[30] = SERVER_SOCKET_PATTERN;
    
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
            msg.trace[i - 1] = 0;
            
            char addr[MN] = SERVER_SOCKET_PATTERN;
            reconnectZmqSocket(requester, to + MIN_ADDR, addr);
            sendMessage(requester, &msg);
            receiveMessage(requester, &msg);
            
            sendMessage(responder, &msg);
            continue;
        }

        if (msg.cmd == CHANGE_ROLE) {
            isParent = 1 - isParent;
            msg.error = 0;
            msg.cmd = DONE;
            sendMessage(responder, &msg);
            continue;
        }

        if (msg.cmd == DELETE_CHILD || msg.cmd == DELETE_PARENT) {
            msg.error = 0;
            msg.cmd = DONE;
            sendMessage(responder, &msg);
            break;
        }

        if (msg.cmd == CMD_START && !isParent) {
            start = clock();
            stop = -1;
            msg.cmd = DONE;
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
            msg.cmd = DONE;
            sendMessage(responder, &msg);
            continue;
        }

        if (msg.cmd == CMD_TIME && !isParent) {
            if (start != -1 && stop == -1) {
                msg.time =  timer + clock() - start;
            } else {
                msg.time = timer;
            }
            msg.cmd = DONE;
            sendMessage(responder, &msg);
            continue;
        }
        msg.cmd = NOTDONE;
        sendMessage(responder, &msg);
    }
    closeZmqSocket(responder);
    printf("[%d]: by by\n", getpid());
    return 0;
}