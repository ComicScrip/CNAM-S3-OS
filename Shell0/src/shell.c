#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

#include "../include/dictionary.h"
#include "../include/shell.h"
#include "../include/utils.h"

shell* shell_create(char** env) {
  shell* s = calloc(1, sizeof(shell));
  s->environement_variables = dictionary_create();
  s->user_varibales = dictionary_create();
  s->special_variables = dictionary_create();

  if(env != NULL){
    // copying the whole environment
    int env_var_parts_nb = 0;
    char** env_var_parts = NULL;

    for (int i = 0; env[i] != NULL; i++) {
      env_var_parts = split(env[i], '=', &env_var_parts_nb, 2);
      if(env_var_parts_nb >= 2) {
        shell_set_environment_var(s, env_var_parts[0], env_var_parts[1]);
      }
    }
  }

  char current_pid[] = {0,0,0,0,0,0,0};
  sprintf(current_pid, "%d", getpid());
  shell_set_special_var(s, "$$", current_pid);
  
  return s;
}

void shell_destroy(shell* s) {
  dictionary_destroy(s->environement_variables);
  dictionary_destroy(s->user_varibales);
  dictionary_destroy(s->special_variables);

  free(s);
}

void shell_set_environment_var(shell* s, char* name, char* value){
  dictionary_set(s->environement_variables, name, value);
  setenv(name, value, 1);
}

void shell_set_special_var(shell* s, char* name, char* value){
  dictionary_set(s->special_variables, name, value);
}

void shell_set_user_var(shell* s, char* name, char* value){
  dictionary_set(s->user_varibales, name, value);
}

char* shell_get_special_variable(shell* s, char* name){
  return dictionary_get(s->special_variables, name);
}

char* shell_get_variable(shell* s, char* name){
  char * ret = NULL;
  if((ret = dictionary_get(s->special_variables, name))) return ret;
  if((ret = dictionary_get(s->environement_variables, name))) return ret;
  if((ret = dictionary_get(s->user_varibales, name))) return ret;
  return ret;
}
