#include "../include/include.h"
#include "../include/msg_q.h"

int main(){
    void *context = zmq_ctx_new();
    void *socketB = createZmqSocket(context, ZMQ_REQ);
    void *socketC = createZmqSocket(context, ZMQ_REP);
    message msg;
    
    connectZmqSocket(socketB, ADDRESS_A_B);
    bindZmqSocket(socketC, ADDRESS_A_C);

    receiveMessage(socketC, &msg);
    if (msg.code == CODE_CONNECT) {
        printf("connect with C\n");
    } else {
        printf("connection lost with C\n");
        sleep(1);
        return 1;
    }  

    char buffer[100];
    while (strcmp(buffer, "exit")) {
        gets(&buffer);
        strcpy(&(msg.str), &buffer);
        if (strcmp(&buffer, "exit") == 0) {
            msg.code = CODE_CLOSE;
        }
        
        sendMessage(socketC, &msg);
        receiveMessage(socketC, &msg);  
        if (msg.code != CODE_OK) {
            printf("not OK from C");
            break;
        }  

        msg.size = 0;
        while (buffer[msg.size] != '\0') {
            msg.size++;
        } 
        if (strcmp(&buffer, "exit") == 0) {
            msg.code = CODE_CLOSE;
        }
        sendMessage(socketB, &msg);
        receiveMessage(socketB, &msg);
    }

    disconnectZmqSocket(socketB, ADDRESS_A_B);
    closeZmqSocket(socketC);
    
    if (_CrtDumpMemoryLeaks())
        printf("memory leak\n");
    else
        printf("all is ok\n");
    sleep(2);
    return 0;
}