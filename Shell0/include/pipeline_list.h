
#ifndef pipeline_list_H
#define pipeline_list_H

#include <string.h>

#include "../include/list.h"
#include "../include/pipeline.h"

/**
 * @defgroup shell0_pipeline_list pipeline_list
 * This module is reponsible for defining structures and operations arround the shell notion of a 'pipeline_list'
 *
 * pipeline lists are one or more pipeline_lists (separated by the the caracters '\n', ';', "&&" or "||"
 *
 * Exemple of pipeline_lists :
 * @code
 * echo "ok" | grep "k" && echo "it works" || echo "this shell is weird"
 * @endcode
 *
 * @{
 *
*/

/**
 * @struct pipeline_list
 * Represents a pipeline_list
 */
typedef struct pipeline_list {
    list* pipelines; ///< A list of the command's redirections. The list_items' data is of type simple_command*
} pipeline_list;

/**
 * Creates an empty pipeline_list instance in heap
 * @return A pointer to the newly allocated instance of the pipeline_list
 */
pipeline_list* pipeline_list_create();

/**
 * Deletes a pipeline_list, freeing its resources
 * @param  cmd  A pointer to the pipeline_list to destroy
 */
void pipeline_list_destroy(pipeline_list* pl);

/**
 * Adds a command to the pipeline_list to be executed.
 *
 * @param pl The pipeline_list to add the command to
 * @param sc The simple command to be added
 */
void pipeline_list_add_pipeline(pipeline_list* pl, pipeline* p);

/**
 * Allows to iterate over the pipeline_list's simple command
 * The next command is returned everytime the function is called
 * @return The next command of the pipeline_list
 */

pipeline* pipeline_list_get_next_pipeline(pipeline_list* pl);

/**
 * Creates a pipeline_list instance from a string representation
 * @param  str The string representation of the pipeline_list
 * For example 'ls -ali /home | grep file && echo "ok"'
 * @return A pointer to a pipeline_list structure
 */
pipeline_list* pipeline_list_from_string(char* str);

/** @} */ // end of pipeline_list module
#endif //pipeline_list_H
