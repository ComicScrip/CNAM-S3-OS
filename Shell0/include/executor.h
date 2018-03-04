#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "../include/simple_command.h"
#include "../include/pipeline.h"
#include "../include/pipeline_list.h"
#include "../include/shell.h"


/**
 * @defgroup shell0_executor Executor
 * This module is reponsible for defining functions relative to the execution of parts of a shell script
 *
 * @{
 *
*/

/**
 * Executes a simple command with its arguments
 * @param char* cmd The binary path
 * @param char* args The arguments string that will be passed to the binary
 * @return int The return status of the command
 */
void execute_simple_command(simple_command* sc);

/**
 * Apply all the command's redirections to the latter
 * @param  sc The simple command to apply redirections to
 */
void apply_redirections(simple_command* sc);

/**
 * Executes a pipeline and optionnaly wait for the latter to complete before proceeding
 * @param  p The pipeline to execute
 * @param async 1 if the command should be executed asynchronously, 0 otherwise
 */
void execute_pipeline(pipeline* p, int async, shell* s);

/**
 * Executes a pipeline list
 * @param  pl The pipeline list to execute
 * @param  s The shell to execute the pipeline list in
 */
void execute_pipeline_list(pipeline_list* pl, shell* s);

/** @} */ // end of executor module
#endif //EXECUTOR_H
