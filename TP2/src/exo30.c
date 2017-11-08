/**
 * \file skeleton.c
 * \brief Basic parsing options skeleton.
 * \author Pierre L. <pierre1.leroy@orange.com>
 * \version 0.1
 * \date 10 septembre 2016
 *
 * Basic parsing options skeleton exemple c file.
 */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>

#include<getopt.h>
#include <fcntl.h>
#include <unistd.h>

#define STDIN 0
#define STDOUT 1
#define STDERR 2

#include <sys/wait.h>

/* Fonction permettant de gerer les cas d'erreur */
void handleError(int check, char * msg) {
    if(check) {
        perror(msg);
        exit(EXIT_FAILURE);
    }
}

void forkError(){
    printf("\nle fork a echoué dans le processus #%d\n", getpid());
    exit(EXIT_FAILURE);
}

void pipeError(){
    perror("le pipe a echoué\n");
    exit(EXIT_FAILURE);
}

void executeGrepProcessus(int tube[2]) {
  // close the pipe's write end
  close(tube[1]);
  // redirect output of grep to the black hole
  int devNull = open("/dev/null", O_WRONLY);
  dup2(STDOUT, devNull);
  // redirect pipe read end to stdin that will feed grep
  dup2(tube[0], STDIN);
  execlp("grep \"^root\"", "grep", NULL);
}

void executePsProcessus(){
  // this process handles "ps eaux | grep "^root " > /dev/null"
  // fork a child that that will read the "ps eaux" command output in a pipe

  int retForkGrep = fork();
  int childExitStatus = -1;

  int tube[2];
  int retPipe = pipe(tube);
  if(retPipe == -1) pipeError();

  switch (retForkGrep){
      case -1: forkError(); break;
      case 0 : executeGrepProcessus(tube); break;
      default:
        // close the read end of the pipe
        close(tube[0]);
        // redirect command output to the tube
        //dup2(STDOUT, tube[1]);
        execlp("ps", "ps", "eaux", NULL);

        waitpid(retForkGrep, &childExitStatus, 0);
        if(childExitStatus == 0) {
          exit(EXIT_SUCCESS);
        } else {
          exit(EXIT_FAILURE);
        }
  }
}

/**
 * Binary main loop
 *
 * \return 1 if it exit successfully
 */
int main(int argc, char** argv)
{
  // this program's aim is to reproduce the following command : "ps eaux | grep "^root " > /dev/null && echo "root est connecté""
  // the main processus will fork a child that handles ps command

  int retFork = fork();
  int childExitStatus = -1;

  switch (retFork){
      case -1: forkError(); break;
      case 0 : executeGrepProcessus(); break;
      default:
        waitpid(retFork, &childExitStatus, 0);
        if(childExitStatus == 0) {
            printf("\nroot est connecté\n");
        }
  }




  return EXIT_SUCCESS;
}
