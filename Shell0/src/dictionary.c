#include <string.h>
#include <stdio.h>
#include "../include/list.h"
#include "../include/dictionary.h"
#include "../include/utils.h"

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
  list_reinit_iteration(d->entries);
}

char** dictionnary_to_string_array(dictionary* d, char sep) {
  list_reinit_iteration(d->entries);
  char** ret = calloc((d->size + 1), sizeof(char*));
  int j, k;

  for(int i = 0; i < d->size; i++) {
    dictionary_entry* de = (dictionary_entry*) list_get_next(d->entries)->data;

    ret[i] = malloc(sizeof(char) * (strlen(de->key) + strlen(de->value) + 2));
    for(j = 0; j < strlen(de->key); j++) ret[i][j] = de->key[j];
    ret[i][j] = sep;
    for(k = 0; k < strlen(de->value); k++) ret[i][j + k + 1] = de->value[k];
    ret[i][j + k + 1] = '\0';
  }

  return ret;
}

dictionary* dictionnary_from_string_array(char** str_array, char sep, int nb_entries){
  dictionary* d = dictionary_create();
  char* str_entry = NULL;
  char** str_entry_parts = NULL;
  char* name = NULL;
  char* value = NULL;
  int nb_parts = 0;

  for(int i=0; i < nb_entries; i++){
    str_entry = str_array[i];
    str_entry_parts = split(str_entry, sep, &nb_parts, 2);
    if(nb_parts == 2){
      name = str_entry_parts[0];
      value = str_entry_parts[1];
      dictionary_set(d, name, value);
    }
    for(int j = 0; j < nb_parts; j++) free(str_entry_parts[j]);
    free(str_entry_parts);
  }

  return d;
}
