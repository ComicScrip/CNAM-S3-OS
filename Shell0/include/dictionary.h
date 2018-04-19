#ifndef DICTIONNARY_H
#define DICTIONNARY_H

#include "../include/list.h"

/**
 * @defgroup dictionnary
 * This module is reponsible for defing a <string, string> dictonnary structure and its operations
 *
 * @{
 *
*/

/**
 * @struct dictionary_entry
 * Represents an entry in a dicionnary
 */
typedef struct dictionary_entry {
    char* key;
    char* value;
} dictionary_entry;

/**
 * @struct dictonnary
 * Represents a dynamic dictionnary whose keys and values are strings
 */
typedef struct dictionary {
  list* entries;
  int size;
} dictionary;

/**
 * Creates a dictionary instannce in heap
 * @return a poiinter to the newly allocated dictionnary
 */
dictionary* dictionary_create();

/**
 * Deletes a dictionnay, freeing its ressources from heap
 */
void dictionary_destroy(dictionary* d);

/**
 * Sets an entry in the dictionnary, with the povided value
 * Keys and values string are copied
 * @param  d The dictonnary to insert the entry into
 * @param  key The key of the entry to be set
 * @param  value The value of the entry to be set
 */
void dictionary_set(dictionary* d, char* key, char* value);

/**
 * Gets value coressponding to key in the dictionnary
 * @method dictionary_get
 * @param  d The dictionnar to search into
 * @param  key The key of the value
 * @return The value coressponding to the key if it exist, NULL otherwise
 */
char* dictionary_get(dictionary* d, char* key);

/**
 * Creates a string array from the dictionnary
 * @param  d The dictionnary to take entries from
 * @param  sep The separator used to separate the key from the value in the strings
 * @return A pointer to a dynamically allocated array of strings or NULL if the dictionnary is empty
 */
char** dictionnary_to_string_array(dictionary* d, char sep);

/**
 * Creates a dictionnary from string array
 * @param  str_array the string array to contruct the dictionary from
 * @param  sep The separator used to separate keys and values
 * @param  nb_enties The number of entries in the array
 * @return A pointer to a dynamically allocated dictionary
 */
dictionary* dictionnary_from_string_array(char** str_array, char sep, int nb_entries);

/** @} */ // end of DICTIONNARY module
#endif //DICTIONNARY_H
