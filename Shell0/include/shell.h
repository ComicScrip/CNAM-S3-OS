#ifndef SHELL_H
#define SHELL_H

#include "../include/dictionary.h"

/**
 * @defgroup shell0_SHELL Shell
 * This module is reponsible for definng shell data structure and operations.
 * This structure is used to memorize all the informations a shell has to memorize across commands executions.
 *
 * @{
 *
*/

/**
 * @struct shell
 * Represents a shell state
 */
typedef struct shell {
  dictionary* environement_variables; ///< a list of key-value pairs that defines the environement of the shell.
  ///< It should be initialized with main's third argument and should be passed to every child process
  ///< These can be set through export
  dictionary* user_varibales; ///< User defined variables for the current shell
  dictionary* special_variables; ///< Contains things like $?, $0, $n, $$ etc
  dictionary* aliases; ///< The command aliases registered for the shell
  list* jobs; ///< Background processes
  pid_t pid;
} shell;

/**
 * Creates a shell state instance in heap
 * @param env The NULL terminated array of strings (that is passed to main as a third arg)
 * @return A pointer to the newly created structure
 */
shell* shell_create(char** env);

/**
 * Destroy the shell state, freeing its allocated resources
 */
void shell_destroy();

/**
 * Sets an environment variable in shell for later use
 * (The variable is exported)
 * @param  s The shell to set the variable in
 * @param  name the name of the variable
 * @param  value The value of the variable
 */
void shell_set_environment_var(shell* s, char* name, char* value);

/**
 * Sets a special variable in shell for later use
 * @param  s The shell to set the variable in
 * @param  name the name of the variable
 * @param  value The value of the variable
 */
void shell_set_special_var(shell* s, char* name, char* value);

/**
 * Sets a user variable in shell for later use
 * @param  s The shell to set the variable in
 * @param  name the name of the variable
 * @param  value The value of the variable
 */
void shell_set_user_var(shell* s, char* name, char* value);

/**
 * Gets a special variable in shell state
 * @param  shell The shell state to perform variable lookup
 * @param name The name of the variable to get
 * @return the value of the variable coressponding to the name. returns NULL if the variable was not set.
 */
char* shell_get_special_variable(shell* s, char* name);

/**
 * Useful for variable substitution. The lookup is done first in special variables table, then environment, then user_defined.
 * @param  shell The shell state to perform variable lookup
 * @param name The name of the variable to get
 * @return the value of the variable coressponding to the name. returns NULL if the variable was not set.
 */
char* shell_get_variable(shell* s, char* name);

/** @} */ // end of SHELL module
#endif //SHELL_H
