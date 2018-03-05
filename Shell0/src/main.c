#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "../include/cli.h"
#include "../include/utils.h"
#include "../include/executor.h"
#include "../include/simple_command.h"
#include "../include/pipeline_list.h"
#include "../include/shell.h"


#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

/**
 Reads its input from a file (see Shell Scripts), from a string supplied as an argument to the -c invocation option (see Invoking Bash), or from the user's terminal.
 Breaks the input into words and operators, obeying the quoting rules described in Quoting. These tokens are separated by metacharacters. Alias expansion is performed by this step (see Aliases).
 Parses the tokens into simple and compound commands (see Shell Commands).
 Performs the various shell expansions (see Shell Expansions), breaking the expanded tokens into lists of filenames (see Filename Expansion) and commands and arguments.
 Performs any necessary redirections (see Redirections) and removes the redirection operators and their operands from the argument list.
 Executes the command (see Executing Commands).
 Optionally waits for the command to complete and collects its exit status (see Exit Status).
**/

/**
 * Binary main loop
 * @return EXIT_SUCCESS if it exit successfully
 */
int main(int argc, char** argv)
{
    //char * user_input;
    while(1) {
      shell * s = shell_create();
      //pipeline_list* pl = pipeline_list_from_string("ls -ali > testfile /home ; grep scrip < testfile && echo ok");
      pipeline_list* pl = pipeline_list_from_string("echo \"\\\">> m test\" | grep test && echo lol");
      //pipeline_list* pl = pipeline_list_from_string("ls -ali /home");
      //execute_simple_command(simple_command_from_string("ls -ali /home"));
        execute_pipeline_list(pl, s);
        break;
        //user_input = get_usr_input();
        //char * user_input = "ls -ali";
        //simple_command * sc = simple_command_from_string(user_input);
        //execute_simple_command(sc);
        //execute_sync(sc);
        //free(user_input);
        //break;
    }

    return EXIT_SUCCESS;
}
