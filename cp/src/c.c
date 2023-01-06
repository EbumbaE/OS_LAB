#include "../include/include.h"
#include "../include/msg_q.h"

int main(){

    void *context = zmq_ctx_new();
    void *socketA = createZmqSocket(context, ZMQ_REQ);
    void *socketB = createZmqSocket(context, ZMQ_REQ);
    message msg;

    connectZmqSocket(socketA, ADDRESS_A_C);
    connectZmqSocket(socketB, ADDRESS_B_C);

    msg.code = CODE_CONNECT;
    sendMessage(socketA, &msg);
    
    int codeExit = 0;
    while(!codeExit){
        receiveMessage(socketA, &msg);
        if(msg.code == CODE_CLOSE){
            codeExit = 1;
        }

        printf("%s\n", msg.str);
        
        msg.code = CODE_OK;
        sendMessage(socketA, &msg);

        msg.size = 0;
        while (msg.str[msg.size] != '\0') {
            msg.size++;
        }
        sendMessage(socketB, &msg);
        receiveMessage(socketB, &msg);
    }
    
    disconnectZmqSocket(socketA, ADDRESS_A_C);
    disconnectZmqSocket(socketB, ADDRESS_B_C);
    
    if (_CrtDumpMemoryLeaks())
        printf("memory leak\n");
    else
        printf("all is ok\n");
    sleep(2);
    return 0;
}