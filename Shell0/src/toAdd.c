#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>



void cd(char* chaine)
{
    chdir(chaine);
}


void pwd()
{
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    printf("Current working dir: %s\n", cwd);
}

void echo(char* chaine)
{
    printf("%s \n", chaine);
}



void exit()
{
    exit(1);
}

