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

void executePsProcessus(int tube[2]){
  // close the read end of the pipe
  close(tube[0]);
  // redirect command output to the pipe
  dup2(tube[1], STDOUT);
  execlp("ps", "ps", "eaux", NULL);
}

void executeGrepProcessus() {

  int tube[2];
  int retPipe = pipe(tube);
  if(retPipe == -1) pipeError();

  int retFork = fork();

  switch (retFork){
      case -1: forkError(); break;
      case 0 : executePsProcessus(tube); break;
      default:
        // close the pipe's write end
        close(tube[1]);
        // redirect output of grep to the black hole
        int devNull = open("/dev/null", O_WRONLY);
        dup2(devNull, STDOUT);
        // redirect pipe read end to stdin that will feed grep
        dup2(tube[0], STDIN);

        //wait(NULL); // wait for child process to write in pipe
        printf("end wait \n");
        execlp("grep", "grep", "\"^root\"", NULL);
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

  int childExitStatus = -1;
  int retFork = fork();

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
