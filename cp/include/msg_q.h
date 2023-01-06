#include <stdlib.h>
#include <errno.h>
#include <zmq.h>
#include <assert.h>

#define MAX_LEN 64
#define MN      30

#define ERR_ZMQ_CTX            100
#define ERR_ZMQ_SOCKET         101
#define ERR_ZMQ_BIND           102
#define ERR_ZMQ_CLOSE          102
#define ERR_ZMQ_CONNECT        104
#define ERR_ZMQ_DISCONNECT     105
#define ERR_ZMQ_MSG            106

#define CODE_OK        200
#define CODE_CONNECT   201
#define CODE_CLOSE     202

typedef struct {
    int code;
    char str[100];
    int size;
} message;

void  clearMessage(message* msg);
void  createAddr(char* addr, int id);
void  bindZmqSocket(void* socket, char* endpoint);

void* createZmqContext();
void* createZmqSocket(void* context, const int type);

void  connectZmqSocket(void* socket, char* endpoint);
void  disconnectZmqSocket(void* socket, char* endpoint);
void  closeZmqSocket(void* socket);
void  deleteZmqContext(void* context);

int receiveMessage(void* socket, message* msg);
int sendMessage(void* socket, message* msg);
int pingProcess(int id);