#include "../include/child.h"

int main() {

    DWORD dwRead;
    char command[10];
    clock_t start = -1, stop = -1, timer = 0;

    HANDLE hStdin, hStdout; 
    hStdout = GetStdHandle(STD_OUTPUT_HANDLE); 
    hStdin = GetStdHandle(STD_INPUT_HANDLE);
    if (hStdout == INVALID_HANDLE_VALUE || hStdin == INVALID_HANDLE_VALUE){
        printf("Error in get pipe in child");
        return 1;
    } 
    
    ReadFile(hStdin, command, sizeof(command), &dwRead, NULL);
    
    while(command[0] != '&') {
        scanf("%s", command);  
        
        if (strcmp(command, "start") == 0) {
            start = clock();
            stop = -1;
        }
        
        if (strcmp(command, "stop") == 0) {
            stop = clock();
            if (start != -1 && stop != -1) { 
                timer += stop - start;
            }
            start = -1;
            stop = -1;
        }

        if (strcmp(command, "time") == 0) {
            if (start != -1 && stop == -1) {
                printf("%d\n", timer + clock() - start);
            } else {
                printf("%d\n", timer); 
            }
        }
        ReadFile(hStdin, command, sizeof(command), &dwRead, NULL);
    }
}