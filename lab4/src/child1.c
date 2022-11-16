#include "../include/work_with_file.h"
#include "../include/work_with_event.h"
#include "../include/include.h"

int main(){

    HANDLE waitEvent = openPublicEvent(&nameWaitChildEndEvent);
    checkNULLEvent(waitEvent);

    HANDLE runChild1Event = openPublicEvent(&nameRunChild1Event);
    checkNULLEvent(runChild1Event);
    HANDLE childTerminateEvent = openPublicEvent(&nameChild1TerminateEvent);
    checkNULLEvent(childTerminateEvent);

    HANDLE runChildEvents[] = {runChild1Event, childTerminateEvent};

    DWORD dwWaitRunChildEvent = WaitForMultipleObjects(
        2,
        runChildEvents, 
        FALSE, 
        INFINITE);

    if (dwWaitRunChildEvent != WAIT_OBJECT_0) {
        CloseHandle(runChildEvents);
        CloseHandle(runChild1Event);
        CloseHandle(childTerminateEvent);
        return 0;
    }

    HANDLE hMapFile = OpenFileMapping(
        FILE_MAP_ALL_ACCESS,
        FALSE,   
        fileMappingName
    );               
    if (hMapFile == NULL) {
        printf("Could not open file mapping object (%d).\n", GetLastError());
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

    DellVowel(pBuf, "../out.txt");

    trySignalToEvent(waitEvent);

    UnmapViewOfFile(pBuf);
    CloseHandle(hMapFile);
    CloseHandle(waitEvent);
    CloseHandle(runChildEvents);
    CloseHandle(runChild1Event);
    CloseHandle(childTerminateEvent);

    return 0;
}