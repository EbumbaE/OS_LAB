#include "../include/work_with_file.h"
#include "stdlib.h"
#include "stdio.h"

int CheckVowel(char c){
    c = tolower(c);
    if ((c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u' || c == 'y')){
        return 0;
    }
    return 1;
}

void DellVowel(char *in, char *out){
    FILE *input = fopen(in, "r");
    if (!input) {
        printf("Error in open input file");
        return;
    }
    FILE *output = fopen(out, "w");
    if (!output) {
        printf("Error in open output file");
        return;
    }

    char c;
    while((c = fgetc(input)) != EOF){
        if (CheckVowel(c)){
            fprintf(output, "%c", c);
        }
    }

    fclose(input);
    fclose(output);
    return;
}
