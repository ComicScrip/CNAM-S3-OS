#include <stdio.h>

int main(int argc, char *argv[], char * envp[])
{
    printf("\nprinting env : \n");
    for (int i = 0; envp[i] != NULL; i++) printf("\n%s", envp[i]);
    printf("\n");
    
    return 0;
}
