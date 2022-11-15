#include "../include/main.h"

int CreateChildProcess(TCHAR *childName, HANDLE handle){
    TCHAR *szCmdline = childName;
    PROCESS_INFORMATION piProcInfo; 
    STARTUPINFO siStartInfo;
    BOOL bSuccess = FALSE; 

    ZeroMemory(&piProcInfo, sizeof(PROCESS_INFORMATION));
    ZeroMemory(&siStartInfo, sizeof(STARTUPINFO));
    siStartInfo.cb = sizeof(STARTUPINFO); 
    siStartInfo.hStdInput = handle;
    siStartInfo.dwFlags |= STARTF_USESTDHANDLES;

    bSuccess = CreateProcess(NULL, szCmdline, NULL, NULL, TRUE, 0, NULL, NULL, &siStartInfo, &piProcInfo); 

    CloseHandle(piProcInfo.hProcess);
    CloseHandle(piProcInfo.hThread);

    return bSuccess;
}

int main() {

    char file1[] = "../in1.txt", file2[] = "../in2.txt";
    // char file1[100], file2[100];
    // printf("Input first file: ");
    // scanf("%s", &file1);
    // printf("Input second file: ");
    // scanf("%s", &file2);

    HANDLE hMapFile;
    LPCTSTR pBuf;

    hMapFile = CreateFileMapping(
       INVALID_HANDLE_VALUE,    
       NULL,                    
       PAGE_READWRITE,          
       0,                       
       BUF_SIZE,                
       fileMappingName
    );                 
    if (hMapFile == NULL) {
        printf("Could not create file mapping object (%d).\n", GetLastError());
        return 1;
    }

    pBuf = (LPTSTR) MapViewOfFile(
        hMapFile,
        FILE_MAP_ALL_ACCESS, 
        0,
        0,
        BUF_SIZE
    );
    if (pBuf == NULL) {
        printf("Could not map view of file (%d).\n", GetLastError());
        CloseHandle(hMapFile);
        return 1;
    }

    int err;
    err = CreateChildProcess(TEXT("../build/CHILD1.exe"), hMapFile);
    if (!err){
        printf("Error in create child1");
        return 1;
    }
    err = CreateChildProcess(TEXT("../build/CHILD2.exe"), hMapFile);
    if (!err){
        printf("Error in create child2");
        return 1;
    }

    TCHAR file[100];

    srand(rand());
    int r = rand() % 100 + 1;
    if (r <= 80) {
        _tcscpy(&file, _T(file1));
        CopyMemory((PVOID)pBuf, file, (_tcslen(file)*sizeof(TCHAR))); 
    }
    else{
        _tcscpy(&file, _T(file2));
        CopyMemory((PVOID)pBuf, file, (_tcslen(file)*sizeof(TCHAR))); 
    }

    sleep(1);
    UnmapViewOfFile(pBuf);
    CloseHandle(hMapFile);
    
    return 0;
}
