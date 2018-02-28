/**
 * \file tp3-ex1.c
 * \brief Compteur de signaux
 * \author Pierre G et Mathilde W
 * \version 0.1
 * \date Février 2018
 *
 * Compteur de signaux
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <getopt.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>


void handlerSIGINT(){

	printf("\nLe programme à été interrompu. Dernière valeur: %d\n", nb);

	cont = 0;

}

void handlerSIGTERM(){

	printf("Fin du programme\n");

	cont = 0;

}


/* Fonction permettant de gerer les cas d'erreur */
void handleError(int check, char * msg) {
    if(check) {
        perror(msg);
        exit(EXIT_FAILURE);
    }
}

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
	
	int i=0;

    // Intercepte les signaux d'interrupteurs et de fin
    signal(SIGINT, handlerSIGINT);
    signal(SIGTERM, handlerSIGTERM);
    printf("Appyez sur Ctrl + C pour arretez le programme\n");
    
    while (cont == 1)
    {
    	// Compteur incrémental
       	nb++;
        if ((nb % 100000000) == 0) 
        {
        	i++;
        	printf("%d\n", nb); 
        	fflush(stdout);
        }
    } 

  return EXIT_SUCCESS;
}
