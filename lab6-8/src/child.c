#include "../include/child.h"

int main() {
    char command[10];
    scanf("%s", command);

    clock_t start = -1, stop = -1, timer = 0;
     
    while(strcmp(command, "exit") != 0) {
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
        scanf("%s", command);
    }
}