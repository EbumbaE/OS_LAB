#include "../include/work_with_event.h"
#include "stdlib.h"
#include "stdio.h"

HANDLE openPublicEvent(LPTSTR name){
    return OpenEvent( 
        EVENT_ALL_ACCESS,               
        FALSE,              
        name
    ); 
}

HANDLE createPublicEvent(LPTSTR name){
    return CreateEvent( 
        NULL,               
        FALSE,               
        FALSE,              
        name
    ); 
}

void checkNULLEvent(HANDLE event){
    if (event == NULL){
        printf("null event (%d)", GetLastError());
        FatalExit(GetLastError());
    }
}

void trySignalToEvent(HANDLE event){
    if (SetEvent(event) == 0){
        printf("Could not to signal in event");
    }
}
