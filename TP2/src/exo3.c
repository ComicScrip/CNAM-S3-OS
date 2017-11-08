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

#include <dirent.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>



//#include <ptdlib.h>

#define STDOUT 1
#define STDERR 2

#define MAX_PATH_LENGTH 4096


#define USAGE_SYNTAX "[OPTIONS]"
#define USAGE_PARAMS "OPTIONS:\n\
  -p, --pathname  PATH_NAME  : the path of the directory you want to ls\n\
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
  { "pathname",   required_argument, 0, 'p' },
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

const short PATH_MAX_LENGTH = 256;

// concatenate two path parts into dest
// ex : pathJoin("dir1/dir2", "file", dest) => dest == "dir1/dir2/file"
void pathJoin(char* part1, char* part2, char dest[PATH_MAX_LENGTH]) {
  size_t part1Length = strlen(part1);
  if(part1Length == 0) return;

  // clear dest str
  for(int i = 0; i < PATH_MAX_LENGTH; i++){
    if(dest[i] == '\0') break;
    else dest[i] = '\0';
  }

  strcpy(dest, part1);
  if(part1[part1Length - 1] != '/'){ // if trailing slash is not there, add it
    dest[part1Length] = '/';
  }
  strcat(dest, part2);
}

void mode_t2str(mode_t m, char str[11]){
    // directory ?
  str[0] = S_ISDIR(m) ? 'd' : '-';
  // user permissions
  str[1] = (m & S_IRUSR) ? 'r' : '-';
  str[2] = (m & S_IWUSR) ? 'w' : '-';
  str[3] = (m & S_IXUSR) ? 'x' : '-';
  // group permissions
  str[4] = (m & S_IRGRP) ? 'r' : '-';
  str[5] = (m & S_IWGRP) ? 'w' : '-';
  str[6] = (m & S_IXGRP) ? 'x' : '-';
  // others permissions
  str[7] = (m & S_IROTH) ? 'r' : '-';
  str[8] = (m & S_IWOTH) ? 'w' : '-';
  str[9] = (m & S_IXOTH) ? 'x' : '-';
  str[10] = '\0';
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
  char* bin_pathname_param = NULL;

  // Parsing options
  int opt = -1;
  int opt_idx = -1;

  while ((opt = getopt_long(argc, argv, binary_optstr, binary_opts, &opt_idx)) != -1)
  {
    switch (opt)
    {
      case 'p':
        //input param
        if (optarg)
        {
          bin_pathname_param = dup_optarg_str();
        }
        break;
      case 'h':
        print_usage(argv[0]);
        free_if_needed(bin_pathname_param);

        exit(EXIT_SUCCESS);
      default :
        break;
    }
  }

  /**
   * Checking binary requirements
   * (could defined in a separate function)
   */
  if (bin_pathname_param == NULL)
  {
    dprintf(STDERR, "Bad usage! See HELP [--help|-h]\n");

    // Freeing allocated data
    free_if_needed(bin_pathname_param);
    // Exiting with a failure ERROR CODE  super-awesome(== 1)
    exit(EXIT_FAILURE);
  }


  // Printing params
  /*
  dprintf(1, "** PARAMS **\n%-8s: %s\n",
          "path",   bin_pathname_param
  );
  */

  // Business logic must be implemented at this point

  DIR* ptdir;
  struct dirent* entree = NULL;
  struct stat entreeStat;
  struct passwd* entreePasswd = NULL;
  struct group* entreeGroup = NULL;
  struct tm* entreeModifTime = NULL;
  char entreePath[PATH_MAX_LENGTH];
  char permissionsStr[11];
  char userGroupName[255];
  char dateModif[255];

  ptdir=opendir(bin_pathname_param);
  handleError(!ptdir, "Problème lors de l'accès au chemin spécifié");

  while((entree=readdir(ptdir))!= NULL)
  {
    pathJoin(bin_pathname_param, entree->d_name, entreePath);

    if(stat(entreePath, &entreeStat) != -1){
      mode_t2str(entreeStat.st_mode, permissionsStr);

      entreePasswd = getpwuid(entreeStat.st_uid);
      entreeGroup = getgrgid(entreeStat.st_gid);
      strcpy(userGroupName, entreePasswd->pw_name);
      strcat(userGroupName, ":");
      strcat(userGroupName, entreeGroup->gr_name);

      entreeModifTime = localtime(&entreeStat.st_mtime);
      strftime(dateModif, 255, "%x @ %X", entreeModifTime);

      printf("%*s -- %*s %*s -- %*d -- %*s\n", 20 , entree->d_name, 20, permissionsStr, 20, userGroupName, 10, (int)entreeStat.st_size, 20, dateModif);
    } else {
      perror("stat error");
    }
  }

  closedir(ptdir);

  // Freeing allocated data
  free_if_needed(bin_pathname_param);
  return EXIT_SUCCESS;
}
