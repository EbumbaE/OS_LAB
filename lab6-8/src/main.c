#include "../include/include.h"
#include "../include/orchestra.h"

int main(){
    
    printf("main [%d] has been created\n\n", getpid());

    printMenu();

    int id_orchestra = 0, err, pid;
    err = OrchestraHandleRequest(id_orchestra, &pid);
    errorHandle(err, pid);

    void *context = createZmqContext();
    void *requester = createZmqSocket(context, ZMQ_REQ);
    char addr[MN] = SERVER_SOCKET_PATTERN;
    reconnectZmqSocket(requester, id_orchestra + MIN_ADDR, addr);
    message msg;

    char command[20];
    int parentID, childID, t, id;
    while(1) {
        scanf("%s", &command);
        msg.error = 0;

        if (strcmp(&command, "exit") == 0) {
            msg.cmd = EXIT;
            sendMessage(requester, &msg);
            receiveMessage(requester, &msg);
            break;
        }

        if (strcmp(&command, "create") == 0) {
            scanf("%s", &command);
            if (strcmp(&command, "child") == 0) {
                scanf("%d %d", &childID, &parentID);
                
                msg.cmd = CREATE_CHILD;
                msg.childID = childID;
                msg.parentID = parentID;
                sendMessage(requester, &msg);
                
                receiveMessage(requester, &msg);
                errorHandle(msg.error, msg.pid);

                continue;
            } 
            if (strcmp(&command, "parent") == 0) {
                scanf("%d", &parentID);
                
                msg.cmd = CREATE_PARENT;
                msg.parentID = parentID;
                sendMessage(requester, &msg);
                
                receiveMessage(requester, &msg);
                errorHandle(msg.error, msg.pid);

                continue;
            }  
        }
        
        if (strcmp(command, "remove") == 0) {
            scanf("%s", command);
            if (strcmp(command, "child") == 0) {
                scanf("%d %d", &childID, &parentID);
                
                msg.cmd = DELETE_CHILD;
                msg.childID = childID;
                msg.parentID = parentID;
                sendMessage(requester, &msg);
                
                receiveMessage(requester, &msg);
                errorHandle(msg.error, msg.pid);

                continue;
            } 
            if (strcmp(command, "parent") == 0) {
                scanf("%d", &parentID);
                
                msg.cmd = DELETE_PARENT;
                msg.parentID = parentID;
                sendMessage(requester, &msg);
                
                receiveMessage(requester, &msg);
                errorHandle(msg.error, msg.pid);

                continue;
            }
        }

        if (strcmp(command, "exec") == 0) {
            scanf("%d %s", &childID, &command);
            msg.childID = childID;
            if (strcmp(&command, "start") == 0) {
                
                msg.cmd = CMD_START;
                sendMessage(requester, &msg);
                
                receiveMessage(requester, &msg);
                errorHandle(msg.error, msg.pid);

                continue;
            } 
            if (strcmp(&command, "stop") == 0) {

                msg.cmd = CMD_STOP;
                sendMessage(requester, &msg);
                
                receiveMessage(requester, &msg);
                errorHandle(msg.error, msg.pid);

                continue;
            } 
            if (strcmp(&command, "time") == 0) {

                msg.cmd = CMD_TIME;
                sendMessage(requester, &msg);
                
                receiveMessage(requester, &msg);
                errorHandle(msg.error, msg.pid);

                if (msg.error == 0) {
                    printf("%d\n", msg.time);
                } else {
                    errorHandle(msg.error, msg.pid);
                }

                continue;
            } 
        }

        if (strcmp(&command, "ping") == 0) {
            scanf("%d", &id);
            
            msg.cmd = PING_NODE;
            msg.pid = id;
            sendMessage(requester, &msg);
            receiveMessage(requester, &msg);
            errorHandle(msg.error, msg.pid);
            continue;
        }

        if (strcmp(&command, "print") == 0) {
            msg.cmd = PRINT_ORCHESTRA;
            sendMessage(requester, &msg);
            receiveMessage(requester, &msg);
            continue;
        }

        printf("Error in command\n");
    }
    sleep(1);
}

void errorHandle(int error, int pid) {
    switch (error) {
        case 0:
            printf("OK [%d]\n", pid);
            break;

        case ErrorNotFoundParent:
            printf("Error [%d]: Parent not found\n", pid);
            break;
    
        case ErrorParentAlreadyExist:
            printf("Error [%d]: Parent Already exists\n", pid);
            break;
    
        case ErrorNotFoundChild:
            printf("Error [%d]: Child not found\n", pid);
            break;
    
        case ErrorChildAlreadyExist:
            printf("Error [%d]: Child Already exists\n", pid);
            break;
    
        case ErrorInCreateChildProccess:
            printf("Error [%d]: Create Child Proccess\n", pid);
            break;

        case ErrorNotFoundNode:
            printf("Error [%d]: Not Found Node\n", pid);
            break;

        case ErrorIncorrectData:
            printf("Error [%d]: Incorrect Data\n", pid);
            break;

        default:
            printf("Error %d: [%d] Not done\n", error, pid);
            break;
    }
}

void printMenu() {
    printf("All commands:\n");
    printf("1) create child [childID] [parentID]\n");
    printf("2) create parent [parentID]\n");
    printf("3) remove child [childID] [parentID]\n");
    printf("4) remove parent [id]\n");
    printf("5) exec [id] [subcommand] ---- subcommand - start/stop/time\n");
    printf("6) ping [id]\n");
    printf("7) exit\n");
}

int OrchestraHandleRequest(int baseID, int *pid) {
    char argBaseID[MN];
    sprintf(argBaseID, "%d", baseID);
    char *args[] = {"orchestra", argBaseID, NULL}; 
    *pid = fork();
    if (*pid == -1) {
        return ErrorInCreateChildProccess;
    }
    if (*pid == 0) {
        return execv("orchestra", args);
    }
    return 0;
}