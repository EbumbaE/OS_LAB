#include "../include/main.h"

int CreateChildProcess(TCHAR *childName, HANDLE pipe[4]){
    TCHAR *szCmdline = childName;
    PROCESS_INFORMATION piProcInfo; 
    STARTUPINFO siStartInfo;
    BOOL bSuccess = FALSE; 

    ZeroMemory(&piProcInfo, sizeof(PROCESS_INFORMATION));
    ZeroMemory(&siStartInfo, sizeof(STARTUPINFO));
    siStartInfo.cb = sizeof(STARTUPINFO); 
    siStartInfo.hStdInput = pipe[INPUT_READ];
    siStartInfo.dwFlags |= STARTF_USESTDHANDLES;

    bSuccess = CreateProcess(NULL, szCmdline, NULL, NULL, TRUE, 0, NULL, NULL, &siStartInfo, &piProcInfo); 

    CloseHandle(piProcInfo.hProcess);
    CloseHandle(piProcInfo.hThread);

    return bSuccess;
}

int main() {
        
    char file1[100], file2[100];
    printf("Input first file: ");
    scanf("%s", &file1);
    printf("Input second file: ");
    scanf("%s", &file2);

    HANDLE pipe1[4], pipe2[4];

    SECURITY_ATTRIBUTES saAttr; 
    saAttr.nLength = sizeof(SECURITY_ATTRIBUTES); 
    saAttr.bInheritHandle = TRUE; 
    saAttr.lpSecurityDescriptor = NULL; 

    if (!CreatePipe(&pipe1[INPUT_READ], &pipe1[INPUT_WRITE], &saAttr, 0)){
        printf("Error in create stdin pipe1");
        return 1;
    }
    if (!CreatePipe(&pipe2[INPUT_READ], &pipe2[INPUT_WRITE], &saAttr, 0)){
        printf("Error in create stdin pipe2");
        return 1;
    }

    int err;
    err = CreateChildProcess(TEXT("../build/CHILD1.exe"), pipe1);
    if (!err){
        printf("Error in create child1");
        return 1;
    }
    err = CreateChildProcess(TEXT("../build/CHILD2.exe"), pipe2);
    if (!err){
        printf("Error in create child2");
        return 1;
    }

    DWORD dwWritten;
    srand(rand());
    int r = rand() % 100 + 1;
    if (r <= 80) {
        WriteFile(pipe1[INPUT_WRITE], &file1, sizeof(file1), &dwWritten, NULL);
        WriteFile(pipe2[INPUT_WRITE], "&", sizeof("&"), &dwWritten, NULL);
    }
    else{
        WriteFile(pipe2[INPUT_WRITE], &file2, sizeof(file2), &dwWritten, NULL);
        WriteFile(pipe1[INPUT_WRITE], "&", sizeof("&"), &dwWritten, NULL);
    }

    CloseHandle(pipe1[INPUT_WRITE]);
    CloseHandle(pipe1[INPUT_READ]);
    CloseHandle(pipe2[INPUT_WRITE]);
    CloseHandle(pipe2[INPUT_READ]);

    return 0;
}
