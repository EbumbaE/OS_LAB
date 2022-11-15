#include "../include/workWithFile.h"
#include "../include/include.h"

int main(){
    
    HANDLE hMapFile;
    LPCTSTR pBuf;

    hMapFile = OpenFileMapping(
        FILE_MAP_ALL_ACCESS,
        FALSE,   
        fileMappingName
    );               
    if (hMapFile == NULL) {
        printf("Could not open file mapping object (%d).\n", GetLastError());
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

    if (pBuf[0] != '&') {
        DellVowel(pBuf, "../out.txt");
    }

    UnmapViewOfFile(pBuf);
    CloseHandle(hMapFile);

    return 0;
}