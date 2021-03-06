/**
 * \file tp1-exo2.c
 * \brief affichage d'une fichier texte a l'envers
 * \author Pierre G et Mathilde W
 * \version 0.1
 * \date février 2018
 *
 * Affichage d'une fichier texte a l'envers
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

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
  int opt = -1;
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
  if (bin_file_param == NULL)
  {
    dprintf(STDERR, "Bad usage! See HELP [--help|-h]\n");

    // Freeing allocated data
    free_if_needed(bin_file_param);
    // Exiting with a failure ERROR CODE  super-awesome(== 1)
    exit(EXIT_FAILURE);
  }


  // Printing params
  dprintf(1, "** PARAMS **\n%-8s: %s\n",
          "file",   bin_file_param
  );

  // Business logic must be implemented at this point

  int fd_input_file = open(bin_file_param, O_RDONLY);
  handleError(fd_input_file == -1, "Erreur d'ouverture du fichier");
  printf("%s", "file reversed : \n");

  // moving pointer before the last char of the file
  off_t nb_chars = lseek(fd_input_file, -1, SEEK_END) + 1;
  char current_character = '\0';
  off_t offset = 1;
  do {
    read(fd_input_file, &current_character, sizeof(char));
    write(STDOUT, &current_character, sizeof(char));
    lseek(fd_input_file, -offset, SEEK_END);
  } while((offset++) <= nb_chars);

  printf("%c", '\n');
  close(fd_input_file);

  // Freeing allocated data
  free_if_needed(bin_file_param);
  return EXIT_SUCCESS;
}
