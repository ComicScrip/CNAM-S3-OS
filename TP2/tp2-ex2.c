/**
 * \file tp2-ex.c
 * \brief redirection de flux standard
 * \author Pierre G et Mathilde W
 * \version 0.1
 * \date Février 2018
 *
 * redirection de flux standard
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <getopt.h>
#include <fcntl.h>
#include <unistd.h>

#define STDOUT 1
#define STDERR 2


/**
 * Binary main loop
 *
 * \return 1 if it exit successfully
 */
int main(int argc, char** argv)
{
  /**
   * Binary variables
   * (could be defined in a structure)
   */
 char* bin_file_param = NULL;

 
  // Printing params
  dprintf(1, "** PARAMS **\n%-8s: %s\n",
          "file",   bin_file_param
  );

  // Business logic must be implemented at this point

	if(argc <= 1 )
    { 
        dprintf(STDERR, "Saisissez au moins 1 argument.\n");
       	exit(EXIT_FAILURE);
    }
    else
    {
        dprintf(STDOUT, "Le premiere argument saisi est : %s\n", argv[1]);   
    }
    
	int status;
	int pid_fils;
	
	int f = fork();
	printf("fork = %d \n", f);
	
	if (f == 0){
		printf("je suis dans le fils : son pid est %d et ceui de son père est %d \n", getpid(), getppid());
		pid_fils = getpid();
		close(STDERR);
		static char template[] = "/tmp/proc-exerciceXXX";
		char file[25];
		strcpy(file, template);
		int fd = mkstemp(file);
		printf("Le numero du descripteur du fils %s est : %d \n", file, fd);
		execlp (argv[1], argv[1], (void*)0);
		exit(1);
	}
	else
	{
		printf("je suis dans le père : son pid est %d et ceui de son fils est %d \n", getpid(), f);
		wait(&status);
		dprintf(STDOUT, "That's All Folks !\n");
	}
  return EXIT_SUCCESS;
}
