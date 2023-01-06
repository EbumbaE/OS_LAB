#include "../include/include.h"
#include "../include/msg_q.h"

int main(){

    void *context = zmq_ctx_new();
    void *socketA = createZmqSocket(context, ZMQ_REP);
    void *socketC = createZmqSocket(context, ZMQ_REP);
    message msg;
    
    bindZmqSocket(socketA, ADDRESS_A_B);
    bindZmqSocket(socketC, ADDRESS_B_C);

    int size;
    int codeExit = 0;
    while(!codeExit){
        receiveMessage(socketA, &msg);
        if (msg.code == CODE_CLOSE) {
            codeExit = 1;
            printf("code exit\n");
        }
        size = msg.size;
        sendMessage(socketA, &msg);
        printf("size string requested A: %d\n", size);

        receiveMessage(socketC, &msg);
        size = msg.size;
        sendMessage(socketC, &msg);
        printf("size string received C: %d\n", size);
    }

    closeZmqSocket(socketA);
    closeZmqSocket(socketC);

    if (_CrtDumpMemoryLeaks())
        printf("memory leak\n");
    else
        printf("all is ok\n");
    sleep(2);
    return 0;
}