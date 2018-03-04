
#ifndef PIPELINE_H
#define PIPELINE_H

#include <string.h>

#include "../include/list.h"
#include "../include/simple_command.h"

/**
 * @defgroup shell0_pipeline Pipeline
 * This module is reponsible for defining structures and operations arround the shell notion of a 'Pipeline'
 *
 * Pipelines are one or more simple commands (separated by the | symbol connects their input and output)
 *
 * Exemple of pipelines :
 * @code
 * ls /etc | wc -l
 * @endcode
 *
 * @{
 *
*/

typedef enum pipeline_terminating_token {
  ASYNC, ///< execute the next pipeline list only of the exit status of the current one is other than 0
  AND, ///< execute the next pipeline list only of the exit status of the current one is other than 0
  OR, ///< execute the next pipeline list only of the exit status of the current one is other than 0
  SEPARATOR ///< normal separator, do nothing
} pipeline_terminating_token;

/**
 * @struct pipeline
 * Represents a pipeline
 */
typedef struct pipeline {
    list* simple_commands; ///< A list of the command's redirections. The list_items' data is of type simple_command*
    pipeline_terminating_token terminating_token;
    int exit_status_inverted; //< Indicates wether or not the return status of the pipeline should be inverted
    int exit_status; //< The exit status of the pipeline is the exit status of the last command in the pipeline;
} pipeline;

/**
 * Creates an empty pipeline instance in heap
 * @return A pointer to the newly allocated instance of the pipeline
 */
pipeline* pipeline_create();

/**
 * Deletes a pipeline, freeing its resources
 * @param  cmd  A pointer to the pipeline to destroy
 */
void pipeline_destroy(pipeline* p);

/**
 * Adds a command to the pipeline to be executed.
 *
 * @param p The pipeline to add the command to
 * @param sc The simple command to be added
 */
void pipeline_add_simple_command(pipeline* p, simple_command* sc);

/**
 * Allows to iterate over the pipeline's simple command
 * The next command is returned everytime the function is called
 * @return The next command of the pipeline
 */
simple_command* pipeline_get_next_simple_command(pipeline* p);

/**
 * Creates a simple pipeline instance from a string representation
 * @param  str The string rpresentation of the pipeline
 * For example 'ls -ali /home | grep file'
 * @return A pointer to a pipeline structure
 */
pipeline* pipeline_from_string(char* str);

/** @} */ // end of pipeline module
#endif //PIPELINE_H
