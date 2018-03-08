#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/cli.h"
#include "../include/shell.h"

#define PROMPT "> "

void print_prompt(shell* s) {
  //TODO: find why the following line swallow stdout
  //printf("--%s-- %s", shell_get_variable(s, "PWD"), PROMPT);
  printf("%s%s", "", PROMPT);
}

char* get_usr_input(shell* s){
    #define CHUNK 300
    print_prompt(s);

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
