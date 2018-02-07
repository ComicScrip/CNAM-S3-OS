#ifndef LIST_H
#define LIST_H

#include <stdlib.h>

/**
 * @defgroup shell0_list List
 * This module is reponsible for defining a simple and generic simply linked list
 *
 * @{
 *
*/

typedef struct list_item list_item;

/**
 * @struct list_item
 * Represents a generic list item
 */
struct list_item {
  void* data; ///< A generic pointer to the data list element is holding
  list_item* next; ///< A pointer to the list's next element, if any, NULL otherwise
};

/**
 * @struct list
 * Represents a generic list
 */
typedef struct list {
  list_item* head; ///< A pointer to the first element of the list
  list_item* tail; ///< A pointer to the last element of the list
  list_item* current; ///< A pointer to the current element when iterating over the list
  size_t size; ///< The size of the list
} list;

/**
 * Creates an instance of the list in heap
 * @return A pointer to the newly created list
 */
list* list_create();

/**
 * Adds an item to the end of the list
 * @param  l         The list to append the element to
 * @param  i         The element to be added
 */
void list_push(list* l, list_item* i);

/**
 * Destroy a list, freeing its ressources in memory
 * @param  l            The list to destroy
 */
void list_destroy(list* l);

/**
 * Iterates over the list's elements. 1 call = 1 iteration
 * @see list_reinit_iteration
 * @param  l             The list to iterate over
 * @return               The next list element
 */
list_item* list_get_next(list* l);

/**
 * Resets the list iteration cursor to the begining of the list
 * @see list_get_next
 * @param  l  The list whose iteration should be reset
 */
void list_reinit_iteration(list* l);

/** @} */ // end of list module
#endif //LIST_H
