#include "../include/msg_q.h"

void* createZmqContext() {
    void* context = zmq_ctx_new();
    if (context == NULL) {
        fprintf(stderr, "[%d] ", getpid());
        perror("ERROR createZmqContext ");
        sleep(1);
        exit(ERR_ZMQ_CTX);
    }
    return context;
}

void bindZmqSocket(void* socket, char* endpoint) {
    if (zmq_bind(socket, endpoint) != 0) {
        fprintf(stderr, "[%d] ", getpid());
        perror("ERROR bindZmqSocket ");
        sleep(1);
        exit(ERR_ZMQ_BIND);
    }
}

void disconnectZmqSocket(void* socket, char* endpoint) {
    if (zmq_disconnect(socket, endpoint) != 0) {
        fprintf(stderr, "[%d] ", getpid());
        perror("ERROR disconnectZmqSocket ");
        sleep(1);
        exit(ERR_ZMQ_DISCONNECT);
    }
}

void connectZmqSocket(void* socket, char* endpoint) {
    if (zmq_connect(socket, endpoint) != 0) {
        fprintf(stderr, "[%d] ", getpid());
        perror("ERROR connectZmqSocket ");
        sleep(1);
        exit(ERR_ZMQ_CONNECT);
    }
}

void* createZmqSocket(void* context, const int type) {
    void* socket = zmq_socket(context, type);
    if (socket == NULL) {
        fprintf(stderr, "[%d] ", getpid());
        perror("ERROR createZmqSocket ");
        sleep(1);
        exit(ERR_ZMQ_SOCKET);
    }
    return socket;
}

void closeZmqSocket(void* socket) {
    if (zmq_close(socket) != 0) {
        fprintf(stderr, "[%d] ", getpid());
        perror("ERROR closeZmqSocket ");
        sleep(1);
        exit(ERR_ZMQ_CLOSE);
    }
}

void deleteZmqContext(void* context) {
    if (zmq_ctx_destroy(context) != 0) {
        fprintf(stderr, "[%d] ", getpid());
        perror("ERROR deleteZmqContext ");
        sleep(1);
        exit(ERR_ZMQ_CLOSE);
    }
}

int receiveMessage(void* socket, message* msg) {
    zmq_msg_t reply;
    zmq_msg_init(&reply);
    if (zmq_msg_recv(&reply, socket, 0) == -1) {
        zmq_msg_close(&reply);
        fprintf(stderr, "[%d] ", getpid());
        perror("ERROR receiveMessage ");
        sleep(1);
        exit(ERR_ZMQ_MSG);
    }
    (*msg) = *((message*)zmq_msg_data(&reply));
    if (zmq_msg_close(&reply) == -1) {
        fprintf(stderr, "[%d] ", getpid());
        perror("ERROR zmq_msg_close ");
        sleep(1);
        exit(ERR_ZMQ_MSG);
    }
    return 1;
}

int sendMessage(void* socket, message* msg) {
    zmq_msg_t zmqMsg;
    zmq_msg_init(&zmqMsg);
    if (zmq_msg_init_size(&zmqMsg, sizeof(message)) == -1) {
        fprintf(stderr, "[%d] ", getpid());
        perror("ERROR zmq_msg_init ");
        sleep(1);
        exit(ERR_ZMQ_MSG);
    }
    if (zmq_msg_init_data(&zmqMsg, msg, sizeof(message), NULL, NULL) == -1) {
        fprintf(stderr, "[%d] ", getpid());
        perror("ERROR zmq_msg_init ");
        sleep(1);
        exit(ERR_ZMQ_MSG);
    }
    if (zmq_msg_send(&zmqMsg, socket, 0) == -1) {
        zmq_msg_close(&zmqMsg);
        return 0;
    }
    return 1;
}
