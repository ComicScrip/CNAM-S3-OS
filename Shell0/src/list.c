#include <stdlib.h>

#include "../include/list.h"

list* list_create(){
  list* l = calloc(1, sizeof(list));
  l->head = NULL;
  l->tail = NULL;
  l->current = NULL;
  l->size = 0;
  return l;
}

void list_push(list* l, list_item* i){
  if(l->head == NULL) {
    l->head = i;
    l->tail = i;
    l->current = i;
  } else {
    l->tail->next = i;
    l->tail = i;
  }

  l->size++;
}

void list_destroy(list* l) {
  list_item* current = NULL;
  current = l->head;
  list_item* to_del = NULL;
  to_del = l->head;
  while(current != NULL) {
    to_del = current;
    current = current->next;
    free(to_del->data);
    free(to_del);
  }
  free(l);
}

list_item* list_get_next(list* l) {
  list_item* li = l->current;
  if(l->current != NULL) {
    l->current = l->current->next;
  }
  return li;
}

void list_reinit_iteration(list* l) {
  l->current = l->head;
}
