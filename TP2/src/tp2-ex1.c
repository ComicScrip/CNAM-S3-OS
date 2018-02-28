/**
 * \file tp2-ex1.c
 * \brief Génération d'une processus fils
 * \author Pierrre G et Mathilde M
 * \version 0.1
 * \date fevrier 2018
 *
 * Génération d'un processus fils
 */
 
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <getopt.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>


/**
 * Binary main loop
 *
 * \return 1 if it exit successfully
 */
int main(int argc, char** argv)
{

	int status;
	int pid_fils;
	
	int f = fork();
	printf("fork = %d \n", f);
	
	if (f == 0){
		printf("je suis dans le fils : son pid est %d et ceui de son père est %d \n", getpid(), getppid());
		pid_fils = getpid();
		exit(getpid() % 10);
	}
	else
	{
		printf("je suis dans le père : son pid est %d et ceui de son fils est %d \n", getpid(), f);
		wait(&status);
		printf("Code retour du fils : %d \n", WEXITSTATUS(status));
	}
	
	
  return EXIT_SUCCESS;
}
