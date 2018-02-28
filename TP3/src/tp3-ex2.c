/**
 * \file tp3-ex2.c
 * \brief min/max threadé
 * \author Pierre G et Mathilde W
 * \version 0.1
 * \date Février 2018
 *
 * min/max threadé
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <getopt.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <pthread.h>

#define SIZE (int) 1e8

int maxValue = 0x80000000;
int minValue = 0x7FFFFFFF;


/* Fonction permettant de gerer les cas d'erreur */
void handleError(int check, char * msg) {
    if(check) {
        perror(msg);
        exit(EXIT_FAILURE);
    }
}


  struct arg_struct {
		int * tab;
		int size;
	};
	
  
  void creationTab(int * tab)
{
  int i;
  time_t t;
  srand((unsigned) time(&t));

  for(i=0; i < SIZE; i++ )
  {
    tab[i] = rand();
  }
}

void *min(void * arg)
{
  struct arg_struct *args = (struct arg_struct *)arg;
  int i;
  for(i=0; i < args->size; i++ )
  {
    if(minValue > args->tab[i])
      minValue = args->tab[i];
  }
}

void *max(void * arg)
{
  struct arg_struct *args = (struct arg_struct *)arg;
  int i;
  for(i=0; i < args->size; i++ )
  {
    if(maxValue < args->tab[i])
      maxValue = args->tab[i];
  }
}


void createThread(int nbThread, int * tab, void  *(*f)(void *))
{
  pthread_t tabThread[nbThread];
  struct arg_struct args;

  if(nbThread <= 0)
    exit(1);

  args.size = SIZE/nbThread;
  args.tab = tab;
  int i=0, md = SIZE%nbThread;
  if ( md != 0)
  {
    pthread_create(&tabThread[i], NULL, &min,(void *) &args);
    i=1;
  }
  for(; i < nbThread; i++)
  {
    args.tab = tab+(i*args.size+md);
    pthread_create(&tabThread[i], NULL, &min,(void *) &args);
  }
  for(i = 0; i < nbThread; i++)
  {
    pthread_join(tabThread[i], NULL);
  }
}





/**
 * Binary main loop
 *
 * \return 1 if it exit successfully
 */
int main(int argc, char** argv)
{	
	
  int * tab = (int *) malloc(SIZE*sizeof(int));
  creationTab(tab);

  struct arg_struct args;
  args.size = SIZE;
  args.tab = tab;
  
  struct timeval tmpdeb;
  struct timeval tmpfin;
  
  
  
  printf("\nTaille :  %ld \n",SIZE );

  printf("\n0 thread créé\n");
  gettimeofday (&tmpdeb, NULL);
  min((void * )&args);
  gettimeofday (&tmpfin, NULL);
  printf("Min = %d \nTemps de recherche : %ld us\n", minValue,  ((tmpfin.tv_sec - tmpdeb.tv_sec) * 1000000 + tmpfin.tv_usec) - tmpdeb.tv_usec);
  gettimeofday (&tmpdeb, NULL);
  max((void * )&args);
  gettimeofday (&tmpfin, NULL);
  printf("Max = %d \nTemps de recherche : %ld us\n", maxValue, ((tmpfin.tv_sec - tmpdeb.tv_sec) * 1000000 + tmpfin.tv_usec) - tmpdeb.tv_usec);

  printf("\n2 threads créés\n");
  gettimeofday (&tmpdeb, NULL);
  createThread(2, tab, min);
  gettimeofday (&tmpfin, NULL);
  printf("Min = %d \nTemps de recherche : %ld us\n", minValue,  ((tmpfin.tv_sec - tmpdeb.tv_sec) * 1000000 + tmpfin.tv_usec) - tmpdeb.tv_usec);
  gettimeofday (&tmpdeb, NULL);
  createThread(2, tab, max);
  gettimeofday (&tmpfin, NULL);
  printf("Max = %d \nTemps de recherche : %ld us\n", maxValue, ((tmpfin.tv_sec - tmpdeb.tv_sec) * 1000000 + tmpfin.tv_usec) - tmpdeb.tv_usec);

  printf("\n4 threads créés\n");
  gettimeofday (&tmpdeb, NULL);
  createThread(4, tab, min);
  gettimeofday (&tmpfin, NULL);
  printf("Min = %d \nTemps de recherche : %ld us\n", minValue,  ((tmpfin.tv_sec - tmpdeb.tv_sec) * 1000000 + tmpfin.tv_usec) - tmpdeb.tv_usec);
  gettimeofday (&tmpdeb, NULL);
  createThread(4, tab, max);
  gettimeofday (&tmpfin, NULL);
  printf("Max = %d \nTemps de recherche : %ld us\n", maxValue, ((tmpfin.tv_sec - tmpdeb.tv_sec) * 1000000 + tmpfin.tv_usec) - tmpdeb.tv_usec);

  printf("\n8 threads créés\n");
  gettimeofday (&tmpdeb, NULL);
  createThread(8, tab, min);
  gettimeofday (&tmpfin, NULL);
  printf("Min = %d \nTemps de recherche : %ld us\n", minValue,  ((tmpfin.tv_sec - tmpdeb.tv_sec) * 1000000 + tmpfin.tv_usec) - tmpdeb.tv_usec);
  gettimeofday (&tmpdeb, NULL);
  createThread(8, tab, max);
  gettimeofday (&tmpfin, NULL);
  printf("Max = %d \nTemps de recherche : %ld us\n\n", maxValue, ((tmpfin.tv_sec - tmpdeb.tv_sec) * 1000000 + tmpfin.tv_usec) - tmpdeb.tv_usec);
 
  free(tab);
  
  return EXIT_SUCCESS;

}
