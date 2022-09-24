#include "../include/workWithFile.h"
#include "../include/include.h"

int main(){
    
    DWORD dwRead;
    char file[100];
    
    HANDLE hStdin, hStdout; 
    hStdout = GetStdHandle(STD_OUTPUT_HANDLE); 
    hStdin = GetStdHandle(STD_INPUT_HANDLE);
    if (hStdout == INVALID_HANDLE_VALUE || hStdin == INVALID_HANDLE_VALUE){
        printf("Error in get pipe in child");
        return 1;
    } 
    
    ReadFile(hStdin, file, sizeof(file), &dwRead, NULL);
    if (dwRead != 0){
        if (file[0] == '&'){
            return 0;
        }
        DellVowel(file, "../out.txt");
    }

    CloseHandle(hStdout);
    CloseHandle(hStdin);

    return 0;
}