#include "../include/include.h"
#include "../include/orchestra.h"

void errorHandling(int error) {
    switch (error) {
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

        default:
            break;
    }
}

int main(){

    Conductor* conductor = NewConductor();

    printf("All commands:\n");
    printf("1) create child [childID] [parentID]\n");
    printf("2) create parent [parentID]\n");
    printf("3) remove child [childID] [parentID]\n");
    printf("4) remove parent [id]\n");
    printf("5) exec [id] [subcommand] ---- subcommand - start/stop/time\n");
    printf("6) ping [id]\n");
    printf("7) exit\n");
    
    char *command;
    int parentID, childID, t, id;
    int err;
    while (1) {
        printf("Do: ");
        scanf("%s", command);

        if (strcmp(command, "exit") == 0) {
            break;
        }

        if (strcmp(command, "create") == 0) {
            scanf("%s", command);
            if (strcmp(command, "child") == 0) {
                scanf("%d %d", &childID, &parentID);
                err = AddChild(conductor, parentID, childID);
                errorHandling(err);
                continue;
            } 
            if (strcmp(command, "parent") == 0) {
                scanf("%d", &parentID);
                err = AddParent(conductor, parentID);
                errorHandling(err);
                continue;
            }  
        }
        
        if (strcmp(command, "remove") == 0) {
            scanf("%s", command);
            if (strcmp(command, "child") == 0) {
                scanf("%d %d", &childID, &parentID);
                err = DeleteChild(conductor, parentID, childID);
                errorHandling(err);
                continue;
            } 
            if (strcmp(command, "parent") == 0) {
                scanf("%d", &parentID);
                err = DeleteParent(conductor, parentID);
                errorHandling(err);
                continue;
            }
        }

        if (strcmp(command, "exec") == 0) {
            scanf("%d %s", &childID, command);
            if (strcmp(command, "start") == 0) {
                err = StartTimer(conductor, childID);
                errorHandling(err);
                continue;
            } 
            if (strcmp(command, "stop") == 0) {
                err = StopTimer(conductor, childID);
                errorHandling(err);
                continue;
            } 
            if (strcmp(command, "start") == 0) {
                err = GetTime(conductor, childID, &t);
                if (err == 0) {
                    printf("%d\n", t);
                } else {
                    errorHandling(err);
                }
                continue;
            } 
        }

        if (strcmp(command, "ping") == 0) {
            scanf("%d", &id);
            if (PingNode(conductor, id)) {
                printf("OK: 1\n");
            } else {
                printf("Error: Not found\n");
            }
            continue;
        }

        printf("Error in command\n");
    }

    DeleteConductor(conductor);
    
    if (_CrtDumpMemoryLeaks())
        printf("memory leak\n");
    else
        printf("all is ok\n");
    
    sleep(1);
}