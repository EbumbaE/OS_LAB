#include "../include/child.h"
#include "../include/msg_q.h"

int main(int argc, char const *argv[]) {

    if (argc < 2) {
        printf("error: amount argv\n");
        return 1;
    }
    
    int id;
    id = atoi(argv[1]);

    printf("child [%d] has been created\n", getpid());

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
        if (msg.childID != id) {
            int to = msg.trace[0];
            
            int i = 1;
            while(msg.trace[i] != 0) {
                msg.trace[i - 1] = msg.trace[i];
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

        if (msg.cmd == DELETE_CHILD) {
            msg.error = 0;
            msg.cmd = DONE;
            sendMessage(responder, &msg);
            break;
        }

        if (msg.cmd == CMD_START) {
            start = clock();
            stop = -1;
        }
        
        if (msg.cmd == CMD_STOP) {
            stop = clock();
            if (start != -1 && stop != -1) { 
                timer += stop - start;
            }
            start = -1;
            stop = -1;
        }

        if (msg.cmd == CMD_TIME) {
            if (start != -1 && stop == -1) {
                msg.time =  timer + clock() - start;
            } else {
                msg.time = timer;
            }
        }

        sendMessage(responder, &msg);
    }
}