#include "../include/main.h"
#include "../include/work_with_event.h"

int CreateChildProcess(TCHAR *childName){
    TCHAR *szCmdline = childName;
    PROCESS_INFORMATION piProcInfo; 
    STARTUPINFO siStartInfo;
    BOOL bSuccess = FALSE; 

    ZeroMemory(&piProcInfo, sizeof(PROCESS_INFORMATION));
    ZeroMemory(&siStartInfo, sizeof(STARTUPINFO));
    siStartInfo.cb = sizeof(STARTUPINFO); 

    bSuccess = CreateProcess(NULL, szCmdline, NULL, NULL, TRUE, 0, NULL, NULL, &siStartInfo, &piProcInfo); 

    CloseHandle(piProcInfo.hProcess);
    CloseHandle(piProcInfo.hThread);

    return bSuccess;
}

int main() {

    _CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);
    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);

    char file1[] = "../in1.txt", file2[] = "../in2.txt";
    // char file1[100], file2[100];
    // printf("Input first file: ");
    // scanf("%s", &file1);
    // printf("Input second file: ");
    // scanf("%s", &file2);

    HANDLE hMapFile = CreateFileMapping(
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

    LPCTSTR pBuf = (LPTSTR) MapViewOfFile(
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

    HANDLE waitChildEndEvent = createPublicEvent(&nameWaitChildEndEvent);
    checkNULLEvent(waitChildEndEvent);

    HANDLE runChild1Event = createPublicEvent(&nameRunChild1Event);
    checkNULLEvent(runChild1Event);

    HANDLE runChild2Event = createPublicEvent(&nameRunChild2Event);
    checkNULLEvent(runChild2Event);

    HANDLE child1TerminateEvent = createPublicEvent(&nameChild1TerminateEvent);
    checkNULLEvent(child1TerminateEvent);

    HANDLE child2TerminateEvent = createPublicEvent(&nameChild2TerminateEvent);
    checkNULLEvent(child2TerminateEvent);

    int err;
    err = CreateChildProcess(TEXT("../build/CHILD1.exe"));
    if (!err){
        printf("Error in create child1");
        return 1;
    }
    err = CreateChildProcess(TEXT("../build/CHILD2.exe"));
    if (!err){
        printf("Error in create child2");
        return 1;
    }

    TCHAR file[100];
    srand(time(NULL));
    int r = rand() % 100 + 1;
    if (r <= 80) {
        _tcscpy(&file, file1);
        CopyMemory((PVOID)pBuf, file, (_tcslen(file)*sizeof(TCHAR))); 

        trySignalToEvent(child2TerminateEvent);
        trySignalToEvent(runChild1Event);
    }
    else{
        _tcscpy(&file, file2);
        CopyMemory((PVOID)pBuf, file, (_tcslen(file)*sizeof(TCHAR))); 

        trySignalToEvent(child1TerminateEvent);
        trySignalToEvent(runChild2Event);
    }
    DWORD dwWaitResult = WaitForSingleObject( 
        waitChildEndEvent, 
        INFINITE);

    if (dwWaitResult != WAIT_OBJECT_0) {
        printf("Error in wait child");
        return 1;
    }

    UnmapViewOfFile(pBuf);
    CloseHandle(hMapFile);
    CloseHandle(waitChildEndEvent);
    CloseHandle(runChild1Event);
    CloseHandle(runChild2Event);
    CloseHandle(child1TerminateEvent);
    CloseHandle(child2TerminateEvent);

    if (_CrtDumpMemoryLeaks())
        printf("memory leak\n");
    else
        printf("all is ok\n");
    
    sleep(5);

    return 0;
}
