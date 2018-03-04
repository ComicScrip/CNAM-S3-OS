#include <string.h>
#include "../include/list.h"
#include "../include/dictionary.h"

dictionary* dictionary_create() {
  dictionary* d = malloc(sizeof(dictionary));
  d->entries = list_create();
  d->size = 0;
  return d;
}

void dictionary_destroy(dictionary* d) {
  list_reinit_iteration(d->entries);
  dictionary_entry* current_entry = NULL;
  list_item* li = NULL;
  for(int i = 0; i < d->size; i++) {
    li = list_get_next(d->entries);
    if(li) current_entry = (dictionary_entry*) li->data;
    free(current_entry->key);
    free(current_entry->value);
  }
  list_destroy(d->entries);
  free(d);
}

dictionary_entry* dictionary_entry_search(dictionary* d, char* key) {
  //printf("\nsearching key %s size%d, ref: %p\n", key, d->size, d);
  list_reinit_iteration(d->entries);
  dictionary_entry* current_entry = NULL;
  list_item* li = NULL;
  for(int i = 0; i < d->size; i++) {
    li = list_get_next(d->entries);
    if(li) current_entry = (dictionary_entry*) li->data;
    if(strcmp(current_entry->key, key) == 0) {
      return current_entry;
    } else {
      current_entry = NULL;
    }
  }
  return current_entry;
}

char* dictionary_get(dictionary* d, char* key) {
  dictionary_entry* de = dictionary_entry_search(d, key);
  if(de == NULL) return NULL;
  else return de->value;
}

void dictionary_set(dictionary* d, char* key, char* value) {
  //printf("\nsetting : k %s v %s\n", key, value);
  int klen, vlen;
  klen = strlen(key); vlen=strlen(value);
  if(klen == 0) return;

  dictionary_entry* de = dictionary_entry_search(d, key);

  if(de == NULL) { // no current_entry for given key
    de = malloc(sizeof(dictionary_entry));
    de->key = malloc(sizeof(char) * klen + 1);
    strcpy(de->key, key);
    de->value = malloc(sizeof(char) * vlen + 1);
    strcpy(de->value, value);
    d->size++;
    list_item* li = calloc(1, sizeof(list_item));
    li->data = de;
    list_push(d->entries, li);
    //printf("\nafter set %p\n", d);
  } else { // there is alredy an entry for the key
    de->value = realloc(de->value, sizeof(char) * vlen + 1);
    strcpy(de->value, value);
  }
}
