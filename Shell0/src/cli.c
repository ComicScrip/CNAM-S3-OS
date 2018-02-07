#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/cli.h"

#define PROMPT ">  "

void print_prompt() {
    printf("%s", PROMPT);
}

char* get_usr_input(){
    #define CHUNK 300
    print_prompt();

    char* input = calloc(CHUNK, sizeof(char));
    char tempbuf[CHUNK];
    size_t inputlen = 0, templen = 0;

    do {
        fgets(tempbuf, CHUNK, stdin);
        templen = strlen(tempbuf);
        inputlen += templen;
        input = realloc(input, (inputlen+1) * sizeof(char));
        strcat(input, tempbuf);
    } while (templen==CHUNK-1 && tempbuf[CHUNK-2]!='\n');

    return input;
}
