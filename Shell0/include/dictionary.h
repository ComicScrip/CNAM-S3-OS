#ifndef DICTIONNARY_H
#define DICTIONNARY_H

#include "../include/list.h"

/**
 * @defgroup shell0_dictionnary
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


/** @} */ // end of DICTIONNARY module
#endif //DICTIONNARY_H
