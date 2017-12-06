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

#define STDOUT 1
#define STDERR 2

#define MAX_PATH_LENGTH 4096


#define USAGE_SYNTAX "[OPTIONS] -i INPUT -o OUTPUT"
#define USAGE_PARAMS "OPTIONS:\n\
  -f, --filename  INPUT_FILE  : input file\n\
***\n\
  -h, --help    : display this help\n\
"

/**
 * Procedure which displays binary usage
 * by printing on stdout all available options
 *
 * \return void
 */
void print_usage(char* bin_name)
{
  dprintf(1, "USAGE: %s %s\n\n%s\n", bin_name, USAGE_SYNTAX, USAGE_PARAMS);
}


/**
 * Procedure checks if variable must be free
 * (check: ptr != NULL)
 *
 * \param void* to_free pointer to an allocated mem
 * \see man 3 free
 * \return void
 */
void free_if_needed(void* to_free)
{
  if (to_free != NULL) free(to_free);
}


/**
 *
 * \see man 3 strndup
 * \see man 3 perror
 * \return
 */
char* dup_optarg_str()
{
  char* str = NULL;

  if (optarg != NULL)
  {
    str = strndup(optarg, MAX_PATH_LENGTH);

    // Checking if ERRNO is set
    if (str == NULL)
      perror(strerror(errno));
  }

  return str;
}


/**
 * Binary options declaration
 * (must end with {0,0,0,0})
 *
 * \see man 3 getopt_long or getopt
 * \see struct option definition
 */
static struct option binary_opts[] =
{
  { "help",    no_argument,       0, 'h' },
  { "filename",   required_argument, 0, 'f' },
  { 0,         0,                 0,  0  }
};

/** <fcntl.h>
 * Binary options string
 * (linked to optionn declaration)
 *
 * \see man 3 getopt_long or getopt
 */
const char* binary_optstr = "hvi:o:";

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
 char* bin_file_param = NULL;

  // Parsing options
 /* int opt = -1;
  int opt_idx = -1;

  while ((opt = getopt_long(argc, argv, binary_optstr, binary_opts, &opt_idx)) != -1)
  {
    switch (opt)
    {
      case 'f':
        //input param
        if (optarg)
        {
          bin_file_param = dup_optarg_str();
        }
        break;
      case 'h':
        print_usage(argv[0]);
        free_if_needed(bin_file_param);

        exit(EXIT_SUCCESS);
      default :
        break;
    }
  }

  /**
   * Checking binary requirements
   * (could defined in a separate function)
   */
  /*if (bin_file_param == NULL)
  {
    dprintf(STDERR, "Bad usage! See HELP [--help|-h]\n");

    // Freeing allocated data
    free_if_needed(bin_file_param);
    // Exiting with a failure ERROR CODE  super-awesome(== 1)
    exit(EXIT_FAILURE);
  }*/


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
