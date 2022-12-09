#include "../include/include.h"

int main() {

    _CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);
    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);

    char* libs[] = {"liblib1.dll","liblib2.dll"};
    int numLib = 0;

    void* libHandle = NULL;
    libHandle = LoadLibrary(libs[numLib]);
    if (!libHandle) {
        printf("DLL open: %s\n", GetLastError());
        sleep(1);
        return 1;
    }

    float (*Pi)(int k) = GetProcAddress(libHandle,"Pi");
    float (*Square)(float a, float b) = GetProcAddress(libHandle,"Square");

    int stage;
    scanf("%d",&stage);
    while(stage != -1){
        switch (stage) {
            case 0:
                FreeLibrary(libHandle);
                numLib = (numLib + 1) % 2;
                libHandle = LoadLibrary(libs[numLib]);
                Pi = GetProcAddress(libHandle,"Pi");
                Square = GetProcAddress(libHandle,"Square");
                break;

            case 1:
                if (Pi == NULL) {
                    printf("NULL address func: Pi\n");
                    break;
                }
                int k;
                scanf("%d", &k);
                printf("%0.3f\n",(Pi)(k));
                break;
            
            case 2:
                if (Square == NULL) {
                    printf("NULL address func: Square\n");
                    break;
                }
                float a, b;
                scanf("%f %f", &a, &b);
                printf("%0.3f\n",(Square)(a, b));
                break;

            default:
                printf("0 - change num lib\n1 arg - Pi(k)\n2 arg1 arg2 - Square(a, b)\n-1 - exit\n");
                break;
        }
        scanf("%d",&stage);
    }
    FreeLibrary(libHandle);

    if (_CrtDumpMemoryLeaks())
        printf("memory leak\n");
    else
        printf("all is ok\n");
    sleep(1);

    return 0;
}
