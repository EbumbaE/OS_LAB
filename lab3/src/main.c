#include "../include/main.h"

#define MAX_THREADS 32
#define MAX_ARRAY 32

int amountTreads, amountElements, countTreads = 0;
int array[MAX_ARRAY];

int fdpipe[2];
enum PIPES { READ, WRITE };

int main(int argc, char *argv[]) {

    _CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);
    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
    
    srand(time(NULL));
    
    if(_pipe(fdpipe, sizeof(int), O_BINARY) == -1)
        perror ("create pipe");

    printf("Input amount treads: ");
    scanf("%d", &amountTreads);
    if (amountTreads > MAX_THREADS)
        amountTreads = MAX_THREADS;
    
    printf("Input amount elements: ");
    scanf("%d", &amountElements);
    if (amountElements >= MAX_ARRAY)
        perror ("to match elements");

    if (amountElements / amountTreads < 2)
        amountTreads = amountElements / 2 + amountElements % 2;

    int count = 0;
    int *begin, *end;   
    for (int i = 0; i < amountElements; i++){
        scanf("%d", &array[i]);
        if (count == 0){
            begin = array; begin += i;
        }
        if (count == amountElements / amountTreads - 1 && countTreads < amountTreads - 1){
            end = array; end += i;
            count = -1;
            countTreads++;

            int* arglist = (int*)malloc(3*sizeof(int));
            arglist[0] = begin; arglist[1] = end; arglist[2] = countTreads;
            _beginthread(minInSublist, 0, (void *)arglist);
        } 
        count++;
    }

    end = array; end += (amountElements - 1);
    countTreads++;
    
    int* arglist = (int*)malloc(3*sizeof(int));
    arglist[0] = begin; arglist[1] = end; arglist[2] = countTreads;
    _beginthread(minInSublist, 0, (void *)arglist);


    int min, buff, err;    
    
    err = _read(fdpipe[READ], &min, sizeof(int));
    printf("tread min: %d\n", min);
    if (!err){
        printf("error in 1 read from thread");
    }
    for(int i = 1; i < amountTreads; i++){
        err = _read(fdpipe[READ], &buff, sizeof(int));
        if (!err){
            printf("error in %d read from thread", i + 1);
        }
        if (min > buff)
            min = buff;
        printf("tread min: %d\n", buff);
    }

    printf("\nresult: %d\n", min);

    _close(fdpipe[READ]);
    _close(fdpipe[WRITE]);

    if (_CrtDumpMemoryLeaks())
        printf("memory leak\n");
    else
        printf("all is ok\n");

    return 0;
}

int minInSublist(void *listarg){
    
    int* args; args = (int *)listarg;
    int* begin = args[0], *end = args[1];

    int *numTread = args[2];
    printf("in %d thread\n", numTread);

    int min = *begin; begin++;
    while (begin <= end){
        if (min > *begin){
            min = *begin;
        }
        begin++;
    }

    _write(fdpipe[WRITE], (char*)&min, sizeof(int));

    free(args);
    return 0;
}