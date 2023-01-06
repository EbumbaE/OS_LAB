#include "../include/parent.h"
#include "../include/msg_q.h"

int main() {
    DWORD dwRead;
    HANDLE hStdin; 
    hStdin = GetStdHandle(STD_INPUT_HANDLE);
    if (hStdin == INVALID_HANDLE_VALUE){
        printf("Error in get pipe in child");
        return 1;
    } 
    int getID[3];
    ReadFile(hStdin, getID, sizeof(getID), &dwRead, NULL);
    int id = getID[0], childLeftID = getID[1], childRightID = getID[2];

    void *context = createZmqContext();
    void *responder = createZmqSocket(context, ZMQ_REP);
    char addr[30] = TCP_SOCKET_PATTERN;
    strcat(addr, id);
    bindZmqSocket(responder, addr);

    printf("id: %d\n", getpid());

    clock_t start = -1, stop = -1, timer = 0;

    message msg;
    
    while(msg.cmd != CMD_EXIT) {

                

    }
}