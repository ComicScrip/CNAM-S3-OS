#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <unistd.h>

#include "../include/cli.h"
#include "../include/utils.h"
#include "../include/executor.h"
#include "../include/simple_command.h"
#include "../include/pipeline_list.h"
#include "../include/shell.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define HISTORY_FILE "command-history.txt"

/**
 * Entry point of the program
 * @return EXIT_SUCCESS if there if evrything went fine
 */
int main(int argc, char** argv, char** env)
{
  printf("\n########################################\n");
  printf("### Shell0 - A null-terminated shell ###\n");
  printf("########################################\n\n");

  char * user_input;
  shell * s = shell_create(env);
   if(argc > 1 && strcmp(argv[1], "-c") == 0  ) {
  	 user_input = argv[2];
  	 if(strcmp(user_input, "exit") == 0){
  		 return EXIT_SUCCESS;
  	 }
     log_command_in_history(HISTORY_FILE, user_input);
     pipeline_list* pl = pipeline_list_from_string(user_input);
     execute_pipeline_list(pl, s);
     pipeline_list_destroy(pl);

     return EXIT_SUCCESS;
   }
   else {
	  while(1) {
      //pipeline_list* pl = pipeline_list_from_string("ls -ali /home");
      //pipeline_list* pl = pipeline_list_from_string("< /home/scrip/.zshrc cat | grep PATH= && echo “path defined in rc” >>file");
  		//pipeline_list*s pl = pipeline_list_from_string("echo \"\\\">> m test\" | grep test && echo ok");
  		//pipeline_list* pl = pipeline_list_from_string("TEST=42 /home/scrip/DATA/CNAM/CNAM-S3-OS/Shell0/printenv | grep TEST");
  		user_input = get_usr_input(s);
      printf("\nuser input:%s|%d\n", user_input, getpid());
  		log_command_in_history(HISTORY_FILE, user_input);
      if(strcmp(user_input, "exit\n") == 0){
        free(user_input);
        printf("\nfinishing...\n");
        return EXIT_SUCCESS;
      } else {
        pipeline_list* pl = pipeline_list_from_string(user_input);
        execute_pipeline_list(pl, s);
        pipeline_list_destroy(pl);
      }
  		free(user_input);
	  }

	  return EXIT_SUCCESS;
	}
}
