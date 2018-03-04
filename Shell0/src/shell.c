#include "../include/dictionary.h"
#include "../include/shell.h"

shell* shell_create(){
  shell* s = calloc(1, sizeof(shell));
  s->variables = dictionary_create();
  return s;
}

void shell_destroy(shell* s) {
  dictionary_destroy(s->variables);
  free(s);
}

void shell_set_var(shell* s, char* name, char* value) {
  dictionary_set(s->variables, name, value);
}

char* shell_get_variable(shell* s, char* name){
  return dictionary_get(s->variables, name);
}
