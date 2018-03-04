#ifndef SHELL_H
#define SHELL_H

#include "../include/dictionary.h"

/**
 * @defgroup shell0_SHELL Shell
 * This module is reponsible for definng shell data structure and operations
 *
 * @{
 *
*/

/**
 * @struct shell
 * Represents a shell state
 */
typedef struct shell {
  dictionary* variables; ///< a list of key string key/value pairs
  list* jobs; ///< background processes
} shell;

/**
 * Creates a shell state instance in heap
 * @return A pointer to the newly created structure
 */
shell* shell_create();

/**
 * Destroy the shell state, freeing its allocated resources
 */
void shell_destroy();

/**
 * Sets a variable in shell for later use
 * @param  s The shell to set the variable in
 * @param  name the name of the variable
 * @param  value The value of the variable
 */
void shell_set_var(shell* s, char* name, char* value);

/**
 * Gets a variable in shell state
 * @param  shell The shell state to perform variable lookup
 * @param name The name of the variable to get
 * @return the value of the variable coressponding to the name. returns NULL if the variable was not set.
 */
char* shell_get_variable(shell* s, char* name);

/** @} */ // end of SHELL module
#endif //SHELL_H
