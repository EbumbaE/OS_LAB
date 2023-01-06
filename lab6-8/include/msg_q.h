#include <unistd.h>
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

#define SERVER_SOCKET_PATTERN   "tcp://localhost:"
#define PING_SOCKET_PATTERN     "inproc://ping"
#define MIN_ADDR                5555

typedef enum {
    CMD_START,
    CMD_STOP,
    CMD_TIME,
    CMD_EXIT,

    CREATE_CHILD,
    CREATE_PARENT,
    DELETE_CHILD,
    DELETE_PARENT,
    PING_NODE,
    EXIT,
    DONE
} cmdType;

typedef struct {
    cmdType     cmd;
    int         time;
    int         parentID, childID;
    int         pid;
    int         error;
    int         trace[100];
} message;

void  createAddr(char* addr, int id);
void  bindZmqSocket(void* socket, char* addr);

void* createZmqContext();
void* createZmqSocket(void* context, const int type);

void  connectZmqSocket(void* socket, char* addr);
void  disconnectZmqSocket(void* socket, char* addr);
void  reconnectZmqSocket(void* socket, int to, char* addr);
void  closeZmqSocket(void* socket);
void  deleteZmqContext(void* context);

int receiveMessage(void* socket, message* msg);
int sendMessage(void* socket, message* msg);
int pingProcess(int id);