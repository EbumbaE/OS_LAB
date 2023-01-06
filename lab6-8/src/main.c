#include "../include/include.h"
#include "../include/orchestra.h"

int main(){
    
    printMenu();

    int id_orchestra = 0;
    int err;
    err = OrchestraHandleRequest(id_orchestra);
    if (err == 0) {
        printf("error to handle request %d", GetLastError());
    }

    printf("user id: %d\n", getpid());

    void *context = createZmqContext();
    void *requester = createZmqSocket(context, ZMQ_REQ);
    char addr[MN] = SERVER_SOCKET_PATTERN;
    reconnectZmqSocket(requester, id_orchestra + MIN_ADDR, addr);
    message msg;
    
    char command[20];
    int parentID, childID, t, id;
    while(1) {
        printf("Do: ");
        scanf("%s", &command);

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
                if (msg.error == 0) {
                    printf("OK: %d", msg.pid);
                } else {
                    errorHandle(msg.error);
                }

                continue;
            } 
            if (strcmp(&command, "parent") == 0) {
                scanf("%d", &parentID);
                
                msg.cmd = CREATE_PARENT;
                msg.parentID = parentID;
                sendMessage(requester, &msg);
                
                receiveMessage(requester, &msg);
                if (msg.error == 0) {
                    printf("OK: %d", msg.pid);
                } else {
                    errorHandle(msg.error);
                }

                continue;
            }  
        }
        
        if (strcmp(command, "remove") == 0) {
            scanf("%s", command);
            if (strcmp(command, "child") == 0) {
                scanf("%d %d", &childID, &parentID);
                
                msg.cmd = DELETE_CHILD;
                msg.childID = childID;
                sendMessage(requester, &msg);
                
                receiveMessage(requester, &msg);
                errorHandle(msg.error);

                continue;
            } 
            if (strcmp(command, "parent") == 0) {
                scanf("%d", &parentID);
                
                msg.cmd = DELETE_PARENT;
                msg.parentID = parentID;
                sendMessage(requester, &msg);
                
                receiveMessage(requester, &msg);
                errorHandle(msg.error);

                continue;
            }
        }

        if (strcmp(command, "exec") == 0) {
            scanf("%d %s", &childID, &command);
            if (strcmp(&command, "start") == 0) {
                
                msg.cmd = CMD_START;
                msg.childID = childID;
                sendMessage(requester, &msg);
                
                receiveMessage(requester, &msg);
                errorHandle(msg.error);

                continue;
            } 
            if (strcmp(&command, "stop") == 0) {

                msg.cmd = CMD_STOP;
                msg.childID = childID;
                sendMessage(requester, &msg);
                
                receiveMessage(requester, &msg);
                errorHandle(msg.error);

                continue;
            } 
            if (strcmp(&command, "time") == 0) {

                msg.cmd = CMD_TIME;
                msg.childID = childID;
                sendMessage(requester, &msg);
                
                receiveMessage(requester, &msg);
                errorHandle(msg.error);

                if (msg.error == 0) {
                    printf("%d\n", msg.time);
                } else {
                    errorHandle(msg.error);
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
            errorHandle(msg.error);
            
            continue;
        }

        printf("Error in command\n");
    }
    
    if (_CrtDumpMemoryLeaks())
        printf("memory leak\n");
    else
        printf("all is ok\n");
    
    sleep(1);
}

void errorHandle(int error) {
    switch (error) {
        case 0:
            printf("Ok: ");
            break;

        case ErrorNotFoundParent:
            printf("Error: Parent not found\n");
            break;
    
        case ErrorParentAlreadyExist:
            printf("Error: Parent Already exists\n");
            break;
    
        case ErrorNotFoundChild:
            printf("Error: Child not found\n");
            break;
    
        case ErrorChildAlreadyExist:
            printf("Error: Child Already exists\n");
            break;
    
        case ErrorInCreateChildProccess:
            printf("Error: Create Child Proccess\n");
            break;

        case ErrorInCreatePipe:
            printf("Error: Create Pipe\n");
            break;

        case ErrorNotFoundNode:
            printf("Error: Not Found Node\n");
            break;

        default:
            printf("Error: %d\n", error);
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

int OrchestraHandleRequest(int baseID) {
    SECURITY_ATTRIBUTES saAttr; 
    saAttr.nLength = sizeof(SECURITY_ATTRIBUTES); 
    saAttr.bInheritHandle = TRUE; 
    saAttr.lpSecurityDescriptor = NULL; 
    HANDLE pipe[2];
    DWORD dwWritten;
    if (!CreatePipe(&pipe[0], &pipe[1], &saAttr, 0)){
        return ErrorInCreatePipe;
    }
    WriteFile(pipe[1], &baseID, sizeof(baseID), &dwWritten, NULL);

    int err = CreateChildProcess(TEXT("ORCHESTRA.exe"), pipe);
    return err;
}

int CreateChildProcess(TCHAR *childName, HANDLE pipe[2]){
    TCHAR *szCmdline = childName;
    PROCESS_INFORMATION piProcInfo; 
    STARTUPINFO siStartInfo;
    BOOL bSuccess = FALSE; 

    ZeroMemory(&piProcInfo, sizeof(PROCESS_INFORMATION));
    ZeroMemory(&siStartInfo, sizeof(STARTUPINFO));
    siStartInfo.cb = sizeof(STARTUPINFO); 
    siStartInfo.hStdInput = pipe[0];
    siStartInfo.dwFlags |= STARTF_USESTDHANDLES;

    bSuccess = CreateProcess(NULL, szCmdline, NULL, NULL, TRUE, 0, NULL, NULL, &siStartInfo, &piProcInfo); 

    CloseHandle(piProcInfo.hProcess);
    CloseHandle(piProcInfo.hThread);

    return bSuccess;
}