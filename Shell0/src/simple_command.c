#include <stdlib.h>
#include <stdio.h>

#include "../include/simple_command.h"
#include "../include/utils.h"

simple_command* simple_command_create() {
  simple_command* sc = (simple_command*) calloc(1, sizeof(simple_command));
  sc->name = calloc(1, sizeof(char));
  sc->name[0] = '\0';
  sc->argv = NULL;
  sc->argc = 0;
  sc->exit_status = -1;
  sc->redirections = calloc(1, sizeof(list));
  sc->env_assignements = calloc(1, sizeof(list));
  return sc;
}

void free_args_if_needed(simple_command* cmd) {
  if(cmd->argc > 0){
    for(int i = 1; i < (cmd->argc + 2); i++){
      free(cmd->argv[i]);
    }
  }
  free(cmd->argv);
}

void simple_command_destroy(simple_command* cmd) {
  list_destroy(cmd->redirections);
  list_destroy(cmd->env_assignements);
  free_args_if_needed(cmd);
  free(cmd->name);
  free(cmd);
}

void simple_command_add_redirection(simple_command* sc, char* redirection_str) {
  list_item* li = calloc(1, sizeof(list_item));
  li->data = calloc(strlen(redirection_str) + 1, sizeof(char));
  strcpy(li->data, redirection_str);
  list_push(sc->redirections, li);
}

void simple_command_add_variable_assignement(simple_command* sc, char* assignement_str){
  list_item* li = calloc(1, sizeof(list_item));
  li->data = calloc(strlen(assignement_str) + 1, sizeof(char));
  strcpy(li->data, assignement_str);
  list_push(sc->env_assignements, li);
}


char* simple_command_get_next_redirection_intent(simple_command* sc){
  list_item* li = list_get_next(sc->redirections);
  return li ? ((char*) li->data) : NULL;
}

char* simple_command_get_next_variable_assignement(simple_command* sc) {
  list_item* li = list_get_next(sc->env_assignements);
  return li ? ((char*) li->data) : NULL;
}

void simple_command_set_args_from_string(simple_command* sc, char* args_str){
  free_args_if_needed(sc);
  int argc = 0;
  char** argument_list = words(args_str, &argc);
  char** argv = (char**) calloc(argc + 2, sizeof(char*)); // +2 for argv[0] and NULL terminating pointer that execv requires
  argv[0] = sc->name;
  for(int i=1; i <= argc; i++) {
    argv[i] = argument_list[i-1];
  }
  argv[argc+1] = NULL;
  sc->argv = argv;
  sc->argc = argc;
  free(argument_list);
}

simple_command* simple_command_from_string(char* str){
  simple_command* sc = simple_command_create();
  int nb_words = 0;
  char** word_list = words(str, &nb_words);
  simple_command_context s = ENV_ASSIGNMENTS;
  char* args_buffer = calloc(1, sizeof(char));
  char* w = NULL;
  char* next_word = NULL;

  for(int i = 0; i < nb_words; i++){
    w = word_list[i];
    switch (s) {
      case ENV_ASSIGNMENTS:
        if(contains(w, "=")){
          simple_command_add_variable_assignement(sc, w);
          break;
        } else {
          s = CMD_NAME;
        }
      case CMD_NAME:
        sc->name = realloc(sc->name, (strlen(w) + 1) * sizeof(char));
        strcpy(sc->name, w);
        // in order to put argv[0] from command name
        simple_command_set_args_from_string(sc, "");
        s = CMD_ARGS;
        break;
      case CMD_ARGS:
        // To know if we are dealing with a redirection token
        // look for redirection operator in non quoted words. TODO: handle non quoted
        if((contains(w, ">") || contains(w, "<"))){
          s = CMD_REDIRECTIONS;
          i--;
        } else {
          args_buffer = realloc(args_buffer, args_buffer == NULL ? (strlen(w) + 2) : (strlen(w) + 2 + strlen(args_buffer)));
          if(strlen(args_buffer)) strcat(args_buffer, " ");
          strcat(args_buffer, w);
        }

        if(s == CMD_REDIRECTIONS || i == (nb_words - 1)) { // we are done with the args
          // flush the args buffer into the struct
          simple_command_set_args_from_string(sc, args_buffer);
        }

        break;
      case CMD_REDIRECTIONS:
        if(!(strcmp(w, "&") == 0)){
          // we can write 'cmd >>file' or 'cmd >> file'
          // in case there's a space we have to look for the next word...
          if(!(i == (nb_words - 1))){ // next word is set only if there is one
            next_word = word_list[i+1];
            if(!(strcmp(next_word, "&") == 0) && !(contains(next_word, "<") || contains(next_word, ">"))){ // next_word is just a filename, lets put it with current redirection
              word_list[i] = realloc(w, sizeof(char) * (strlen(w) + strlen(next_word) + 1));
              w = word_list[i];
              //printf("%s\n", next_word);
              strcat(w, next_word);
              // skip next word for next iteration
              i++;
            }
          } else {
            next_word = "";
          }
          simple_command_add_redirection(sc, w);
        } else {
          sc->bg = 1;
        }

        break;
      }
  }

  free(args_buffer);

  for(int i = 0; i < nb_words; i++){
    free(word_list[i]);
  }
  free(word_list);

  return sc;
}
