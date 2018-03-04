#ifndef UTILS_H
#define UTILS_H

/**
 * @defgroup shell0_utils Utils
 * This module is reponsible for defining utility functions and structures
 *
 * @{
 *
*/

/**
 * Frees malloc'ed data pointed by the pointer in parameters if it points to something
 * @param to_free A pointer ponting to data to free
 */
void free_if_needed(void* to_free);

/**
 * Handles an error by printing a message and exiting the program if check evaluates to true.
 * Exemple usage : @code handle_error(data = malloc(sizeof(int)), "could not allocate memory") @endcode
 * @param check Indicates whether or not the error will be handled
 * @param msg The error message to print to STDERR
 */
void handle_error(int check, char * msg);

/**
 * Finds the index of a sequence of characters in a string.
 *
 * For exemple,
 * @code
 * index_of("Hello world !", "lo", 1) -> 3
 * index_of("Hello world !", "o", 2) -> 7
 * index_of("Hello world !", "@", 1) -> -1
 * @endcode
 *
 * @param  haystack          The string to search into
 * @param  needle            The string to search for
 * @param  occurence_number  The occurence number of the word we want the index of
 * @return                   If the haystack string does not contain the needle string, -1 is returned.
 * Otherwise it will return the index of the *occurence_number*th occurence of
 * needle string in haystack string.
 */
int index_of(char* haystack, char* needle, int occurence_number);

/**
 * Indicates whether or not haystack string contains needle string
 * @param  haystack The string to search into
 * @param  needle   The string to search for
 * @return          1 if haytack contains needle, 0 otherwise
 */
int contains(char* haystack, char* needle);

/**
 * Splits the string in parameters into words
 * (sequences of characters deleimited by one or more blanks, ie: ' ', '\t' '\n')
 * @method words
 * @param  str   The string to split
 * @param  nb_words   An optionnal pointer to the number of words outputed, give NULL if you don't care
 * @return       An array of strings with the words
 */
char** words(char* str, int* nb_words);

/**
 * Reallocate str to append another string afer the latter
 * @param  str The begining of the resulting string
 * @param  to_append The end of the resulting string
 * @return a pointer to the reallocated string containing str and to_append
 */
char* strcat_dyn(char* str, char* to_append);

/**
 * Reallocate str to append a single caracter afer the latter
 * @param  str The begining of the resulting string
 * @param  to_append The character to append
 * @return a pointer to the reallocated string containing str and to_append
 */
char* append(char* str, char to_append);

/**
 * Remove quotes that are not escaped by \ in the string and realocate the string
 * @param  str The string to remove quotes from. MUST BE malloc'd
 * @return a pointer to the reallocated string passed in parameters
 */
char* strip_quotes(char* str);


/** @} */ // end of utils module
#endif //UTILS_H
