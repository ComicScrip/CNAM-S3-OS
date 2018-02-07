#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "../include/simple_command.h"

/**
 * @defgroup shell0_executor Executor
 * This module is reponsible for defining functions relative to the execution of parts of a shell script
 *
 * @{
 *
*/

/**
 * @fn int execute_command(char* cmd, char* args)
 * Executes a simple command with its arguments
 * @param char* cmd The binary path
 * @param char* args The arguments string that will be passed to the binary
 * @return int The return status of the command
 */

void execute_simple_command(simple_command* sc);

void apply_redirections(simple_command* sc);

void execute_sync(simple_command* sc);

/** @} */ // end of executor module
#endif //EXECUTOR_H
