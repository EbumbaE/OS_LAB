#define _CRT_SECURE_NO_WARNINGS
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#include "../include/test_tree.h"

int main() {

    int c, x;
  
    Node* tree = NULL;

    printf("1 - add node\n");
    printf("2 - print tree\n");
    printf("3 - delete node\n");
    printf("4 - count size tree\n");
    printf("5 - count trace\n");
    printf("0 - exit\n");
    
    printf("Do: ");
    scanf("%d", &c);
    while (c != 0) {
        switch (c)
        {
        case 1:
            printf("input id: ");
            scanf("%d", &x);
            tree = insertNode(tree, x);
            break;
        case 2:
            printf("\n");
            printTree(tree);
            printf("\n");
            break;
        case 3:
            printf("input id: ");
            scanf("%d", &x);
            tree = deleteNode(tree, x);
            break;
        case 4:
            printf("%lld\n", countSize(tree));
            break;
        case 5:
            printf("input id: ");
            scanf("%d", &x);
            int trace[100];
            x = countTrace(tree, x, &trace, 0, 0);
            if (x) {
                x = 0;
                while(trace[x] != 0) {
                    printf("%d ", trace[x]);
                    x++;
                }
                printf("\nsize: %d\n", x);
            }
            break;
       default:
            break;
        }
        printf("Do: ");
        scanf("%d", &c);
    }

    deleteTree(tree);
    
    if (_CrtDumpMemoryLeaks())
        printf("memory leak\n");
    else
        printf("all is ok\n");
    
    sleep(1);
}