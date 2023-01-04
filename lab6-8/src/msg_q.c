#include "../include/msg_q.h"

void createAddr(char* addr, int id) {
    char str[MN];
    memset(str, 0, MN);
    sprintf(str, "%d", id);
    memset(addr, 0, MN);
    memcpy(addr, SERVER_SOCKET_PATTERN, sizeof(SERVER_SOCKET_PATTERN));
    strcat(addr, str);
}

void clearMessage(message* msg) {
    msg->cmd = CMD_EXIT;
}

void* createZmqContext() {
    void* context = zmq_ctx_new();
    if (context == NULL) {
        fprintf(stderr, "[%d] ", getpid());
        perror("ERROR createZmqContext ");
        exit(ERR_ZMQ_CTX);
    }
    return context;
}

void bindZmqSocket(void* socket, char* endpoint) {
    if (zmq_bind(socket, endpoint) != 0) {
        fprintf(stderr, "[%d] ", getpid());
        perror("ERROR bindZmqSocket ");
        exit(ERR_ZMQ_BIND);
    }
}

void disconnectZmqSocket(void* socket, char* endpoint) {
    if (zmq_disconnect(socket, endpoint) != 0) {
        fprintf(stderr, "[%d] ", getpid());
        perror("ERROR disconnectZmqSocket ");
        exit(ERR_ZMQ_DISCONNECT);
    }
}

void connectZmqSocket(void* socket, char* endpoint) {
    if (zmq_connect(socket, endpoint) != 0) {
        fprintf(stderr, "[%d] ", getpid());
        perror("ERROR connectZmqSocket ");
        exit(ERR_ZMQ_CONNECT);
    }
}

void* createZmqSocket(void* context, const int type) {
    void* socket = zmq_socket(context, type);
    if (socket == NULL) {
        fprintf(stderr, "[%d] ", getpid());
        perror("ERROR createZmqSocket ");
        exit(ERR_ZMQ_SOCKET);
    }
    return socket;
}

void reconnectZmqSocket(void* socket, int to, char* addr) {
    if (addr[16] != '\0') {
        disconnectZmqSocket(socket, addr);
    }
    createAddr(addr, to);
    connectZmqSocket(socket, addr);
}

void closeZmqSocket(void* socket) {
    if (zmq_close(socket) != 0) {
        fprintf(stderr, "[%d] ", getpid());
        perror("ERROR closeZmqSocket ");
        exit(ERR_ZMQ_CLOSE);
    }
}

void deleteZmqContext(void* context) {
    if (zmq_ctx_destroy(context) != 0) {
        fprintf(stderr, "[%d] ", getpid());
        perror("ERROR deleteZmqContext ");
        exit(ERR_ZMQ_CLOSE);
    }
}

int receiveMessage(void* socket, message* msg) {
    zmq_msg_t reply;
    zmq_msg_init(&reply);
    if (zmq_msg_recv(&reply, socket, 0) == -1) {
        zmq_msg_close(&reply);
        fprintf(stderr, "[%d] ", getpid());
        perror("ERROR zmq_msg_recv ");
        exit(ERR_ZMQ_MSG);
    }
    (*msg) = *((message*)zmq_msg_data(&reply));
    if (zmq_msg_close(&reply) == -1) {
        fprintf(stderr, "[%d] ", getpid());
        perror("ERROR zmq_msg_close ");
        exit(ERR_ZMQ_MSG);
    }
    return true;
}

int sendMessage(void* socket, message* msg) {
    zmq_msg_t zmqMsg;
    zmq_msg_init(&zmqMsg);
    if (zmq_msg_init_size(&zmqMsg, sizeof(message)) == -1) {
        fprintf(stderr, "[%d] ", getpid());
        perror("ERROR zmq_msg_init ");
        exit(ERR_ZMQ_MSG);
    }
    if (zmq_msg_init_data(&zmqMsg, msg, sizeof(message), NULL, NULL) == -1) {
        fprintf(stderr, "[%d] ", getpid());
        perror("ERROR zmq_msg_init ");
        exit(ERR_ZMQ_MSG);
    }
    if (zmq_msg_send(&zmqMsg, socket, 0) == -1) {
        zmq_msg_close(&zmqMsg);
        return false;
    }
    return true;
}

int pingProcess(int id) {
    char addr_monitor[30];
    char addr_connection[30];
    char str[30];
    memset(str, 0, 30);
    sprintf(str, "%d", id);
    memset(addr_monitor, 0, 30);
    memcpy(addr_monitor, PING_SOCKET_PATTERN, sizeof(PING_SOCKET_PATTERN));
    strcat(addr_monitor, str);
    memset(addr_connection, 0, 30);
    memcpy(addr_connection, SERVER_SOCKET_PATTERN, sizeof(SERVER_SOCKET_PATTERN));
    strcat(addr_connection, str);

    void* context = zmq_ctx_new();
    void *requester = zmq_socket(context, ZMQ_REQ);

    zmq_socket_monitor(requester, addr_monitor, ZMQ_EVENT_CONNECTED | ZMQ_EVENT_CONNECT_RETRIED);
    void *socket = zmq_socket(context, ZMQ_PAIR);
    zmq_connect(socket, addr_monitor);
    zmq_connect(requester, addr_connection);

    zmq_msg_t msg;
    zmq_msg_init(&msg);
    zmq_msg_recv(&msg, socket, 0);
    uint8_t* data = (uint8_t*)zmq_msg_data(&msg);
    uint16_t event = *(uint16_t*)(data);

    zmq_close(requester);
    zmq_close(socket);
    zmq_msg_close(&msg);
    zmq_ctx_destroy(context);
    if (event == ZMQ_EVENT_CONNECT_RETRIED) {
        return false;
    } else {
        return true;
    }
}
