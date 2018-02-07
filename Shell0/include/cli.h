#ifndef CLI_H
#define CLI_H

/**
 * @defgroup shell0_cli CLI
 * This module is reponsible for defining functions relative to the Command Line Interface of the program.
 *
 * @{
 *
*/

/**
 * Prints a predefined prompt to STDOUT
 */
void print_prompt();
/**
 * Prompt for user input in STDIN
 * @return a pointer to dynamically allocated user input string
 */
char* get_usr_input();

/** @} */ // end of cli module
#endif //CLI_H
