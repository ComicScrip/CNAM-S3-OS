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


#define STDOUT 1
#define STDERR 2
#define MAX_PATH_LENGTH 4096
#define SHELL_INPUT_SIZE 1000

#define USAGE_SYNTAX "[OPTIONS]"
#define USAGE_PARAMS "OPTIONS:\n\
  -v, --verbose : enable *verbose* mode\n\
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
  { "verbose", no_argument,       0, 'v' },
  { 0,         0,                 0,  0  }
};

/**
 * Binary options string
 * (linked to optionn declaration)
 *
 * \see man 3 getopt_long or getopt
 */
const char* binary_optstr = "hvi:o:";

void parseInput(char* input, char inputParts[SHELL_INPUT_SIZE][SHELL_INPUT_SIZE]){
    for(int inputCurrentIndex, partCharIndex, partIndex = 0; inputCurrentIndex < SHELL_INPUT_SIZE; inputCurrentIndex++){
      inputParts[partIndex][partCharIndex] = input[inputCurrentIndex];
      printf("%c \n", inputParts[partIndex][partCharIndex]);
      partCharIndex++;
      if(input[inputCurrentIndex] == ' '){
        partIndex++;
        partCharIndex = 0;
      } else if (input[inputCurrentIndex] == '\0') {
        break;
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
  /**
   * Binary variables
   * (could be defined in a structure)
   */
  short int is_verbose_mode = 0;

  // Parsing options
  int opt = -1;
  int opt_idx = -1;

  while ((opt = getopt_long(argc, argv, binary_optstr, binary_opts, &opt_idx)) != -1)
  {
    switch (opt)
    {
      case 'v':
        //verbose mode
        is_verbose_mode = 1;
        break;
      case 'h':
        print_usage(argv[0]);

        exit(EXIT_SUCCESS);
      default :
        break;
    }
  }

  /**
   * Checking binary requirements
   * (could defined in a separate function)
   */
/*
  if (bin_input_param == NULL || bin_output_param == NULL)
  {
    dprintf(STDERR, "Bad usage! See HELP [--help|-h]\n");

    // Freeing allocated data
    free_if_needed(bin_input_param);
    free_if_needed(bin_output_param);
    // Exiting with a failure ERROR CODE (== 1)
    exit(EXIT_FAILURE);
  }
*/

  // Printing params
/*
  dprintf(1, "** PARAMS **\n%-8s: %s\n%-8s: %s\n%-8s: %d\n",
          "verbose", is_verbose_mode);
*/
  // Business logic must be implemented at this point

  char miniShellInput[SHELL_INPUT_SIZE];
  while(1){
    printf("Prompt >\n");
    scanf("%s", miniShellInput);
    char parsedInput[SHELL_INPUT_SIZE][SHELL_INPUT_SIZE];
    parseInput(miniShellInput, parsedInput);
    //execlp("ps", "ps", "eaux", NULL);

  }

  // Freeing allocated data
  //free_if_needed(bin_input_param);
  //free_if_needed(bin_output_param);


  return EXIT_SUCCESS;
}
