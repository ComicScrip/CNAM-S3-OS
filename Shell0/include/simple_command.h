
#ifndef SIMPLE_COMMAND_H
#define SIMPLE_COMMAND_H

#include <string.h>

#include "../include/list.h"

/**
 * @defgroup shell0_simple_command Simple Command
 * This module is reponsible for defining structures and operations arround the shell notion of a 'Simple command'
 *
 * A simple command is a sequence of optional variable assignments followed by blank-separated words and redirections,
 * and terminated by a control operator. The first word specifies the command to be executed, and is passed as argument
 * zero.  The remaining words are passed as arguments to the invoked command.
 *
 * Exemple of simple commands :
 * @code
 * ls
 * ls > list.txt
 * ls -l
 * LC_ALL=C ls
 * @endcode
 *
 * @{
 *
*/

/**
 * @struct simple_command
 * Represents a simple command
 */
typedef struct simple_command {
    char* name; ///< The name of the executable, ($PATH search will be performed)
    char** argv; ///< The NULL terminated array of args, the first element of the array is the command name
    int argc; ///< The number of arguments including the command name
    char exit_status; ///< The exit status of the forked program once it is run.
    ///< Exit code range is from 0 to 255, where 0 means success, and the rest mean either something failed, or there is an issue to report back to the calling program
    list* redirections; ///< A list of the command's redirections. The list_items' data is of type char*
    char** env_assignements; ///< Environement variables that should be assigned to the process before its execution. The array is NULL terminated
    int nb_assignments; ///< The number of environment variable assignements
} simple_command;

/**
 * @enum
 * Represents the contexts in simple commands
 */
typedef enum {
  ENV_ASSIGNMENTS,
  CMD_NAME,
  CMD_ARGS,
} simple_command_context;

/**
 * Creates an empty simple command instance in heap
 * @return A pointer to the newly allocated instance of the command
 */
simple_command* simple_command_create();

/**
 * Deletes a simple command, freeing its resources
 * @param  cmd  A pointer to the command to destroy
 */
void simple_command_destroy(simple_command* cmd);

/**
 * Adds a redirection intent to the command to be executed.
 *
 * @param sc The command to add redirection to
 * @param The string representation of the redirection, i.e : '>> file.txt'
 */
void simple_command_add_redirection(simple_command* sc, char* redirection_str);

/**
 * Adds a variable assignment to the command to be executed.
 *
 * @param sc The command to add assignment to
 * @param redirection_str The string representation of the assignement, i.e. : 'RAILS_ENV=production'
 */
void simple_command_add_variable_assignement(simple_command* sc, char* redirection_str);

/**
 * Allows to iterate over the command's redirection intents.
 * The next redirection intent is returned everytime the function is called
 * @return The next redirection intent of the command
 */
char* simple_command_get_next_redirection_intent(simple_command* sc);

/**
 * Sets the argv and argc members for a simple command
 * @param  sc The command
 * @param  args_str The string representation of the command's arguments
 */
void simple_command_set_args_from_string(simple_command* sc, char* args_str);

/**
 * Creates a simple command instance from a string representation
 * @param  str The string rpresentation of the command
 * For example 'ls -ali /home >> file 2>&1'
 * @return A simple command structure
 */
simple_command* simple_command_from_string(char* str);

/** @} */ // end of simple command module
#endif //SIMPLE_COMMAND_H
