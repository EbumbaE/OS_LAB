#include "../include/include.h"

int main(){

    Conductor* conductor = NewConductor();

    printf("All commands:\n");
    printf("1) create child [childID] [parentID]\n");
    printf("2) create parent [parentID]\n");
    printf("3) remove child [childID] [parentID]\n");
    printf("4) remove parent [id]\n");
    printf("5) exec [id] [subcommand] subcommand - start/stop/time\n");
    printf("6) exit\n");
    
    char *command;
    int parentID, childID, t;
    while (strcmp(command, "exit") != 0) {
        printf("Do: ");
        scanf("%s", command);

        if (strcmp(command, "create") == 0) {
            scanf("%s", command);
            if (strcmp(command, "child") == 0) {
                scanf("%d %d", &childID, &parentID);
                AddChild(conductor, parentID, childID);
                continue;
            } 
            if (strcmp(command, "parent") == 0) {
                scanf("%d", &parentID);
                AddParent(conductor, parentID);
                continue;
            }  
        }
        
        if (strcmp(command, "remove") == 0) {
            scanf("%s", command);
            if (strcmp(command, "child") == 0) {
                scanf("%d %d", &childID, &parentID);
                DeleteChild(conductor, parentID, childID);
                continue;
            } 
            if (strcmp(command, "parent") == 0) {
                scanf("%d", &parentID);
                DeleteParent(conductor, parentID);
                continue;
            }
        }

        if (strcmp(command, "exec") == 0) {
            scanf("%d %s", &childID, command);
            if (strcmp(command, "start") == 0) {
                StartTimer(conductor, childID);
                continue;
            } 
            if (strcmp(command, "stop") == 0) {
                StopTimer(conductor, childID);
                continue;
            } 
            if (strcmp(command, "start") == 0) {
                t = GetTime(conductor, childID);
                if (t != -1) {
                    printf("%d\n", t);
                }
                continue;
            } 
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