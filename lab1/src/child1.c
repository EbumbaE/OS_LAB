#include "../include/workWithFile.h"
#include "../include/include.h"

int main(){
    
    DWORD dwRead;
    char file[100];
    
    HANDLE hStdin, hStdout; 
    hStdout = GetStdHandle(STD_OUTPUT_HANDLE); 
    hStdin = GetStdHandle(STD_INPUT_HANDLE);
    if (hStdout == INVALID_HANDLE_VALUE || hStdin == INVALID_HANDLE_VALUE){
        printf("Error in get pipe in child 1");
        return 0;
    } 
    
    ReadFile(hStdin, file, sizeof(file), &dwRead, NULL);
    if (dwRead != 0){
        DellVowel(file, "../out.txt");
    }
    return 1;
}