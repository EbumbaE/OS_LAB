#include "../include/include.h" 
#include "../include/lib.h"

int main() {

    _CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);
    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);

    int stage;
    scanf("%d",&stage);
    while(stage != -1){
        switch (stage) {
            case 1:
                int k;
                scanf("%d", &k);
                printf("%0.3f\n",Pi(k));
                break;
           
            case 2:
                float a, b;
                scanf("%f %f", &a, &b);
                printf("%0.3f\n",Square(a, b));
                break;
                
            default:
                printf("1 arg - Pi(k)\n2 arg1 arg2 - Square(a, b)\n-1 - exit\n");
                break;
        }
        scanf("%d",&stage);
    }

    if (_CrtDumpMemoryLeaks())
        printf("memory leak\n");
    else
        printf("all is ok\n");
    sleep(2);

    return 0;
}
