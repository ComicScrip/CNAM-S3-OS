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
  for(int i = 0; i < cmd->argc; i++){
    free(cmd->argv[i]);
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

void simple_command_add_arg(simple_command* sc, char* arg) {
  sc->argv = realloc(sc->argv, (sc->argc + 2) * sizeof(char*)); // argv needs to be a NULL termiated array
  sc->argv[sc->argc] = calloc(1, sizeof(char) * strlen(arg) + 1);
  sc->argv[sc->argc + 1] = NULL;
  strcpy(sc->argv[sc->argc], arg);
  sc->argc++;
}

simple_command* simple_command_from_string(char* str){
  simple_command* sc = simple_command_create();
  int nb_words = 0;
  char** word_list = words(str, &nb_words);
  simple_command_context s = ENV_ASSIGNMENTS;
  char* args_buffer = calloc(1, sizeof(char));
  char* w = NULL;
  char* next_word = NULL;
  int was_quoted = 0;
  int length_before_quote = 0;

  for(int i = 0; i < nb_words; i++){
    length_before_quote = strlen(word_list[i]);
    w = strip_quotes(word_list[i]); word_list[i] = w;
    was_quoted = length_before_quote != strlen(w);
    if(!was_quoted && (contains(w, ">") || contains(w, "<"))){
      // we can write 'cmd >>file' or 'cmd >> file'
      // in case there's a space we have to look for the next word...
      if (!(i == (nb_words - 1)) && !contains(w, "&") && ((contains(w, ">") &&
        w[strlen(w) -1] == '>') || (contains(w, "<") && w[strlen(w) -1] == '<')))
      { // next word is set only if there is one
        next_word = word_list[i+1];
        if(!(contains(next_word, "<") || contains(next_word, ">"))){ // next_word is just a filename, lets put it with current redirection
          word_list[i] = realloc(w, sizeof(char) * (strlen(w) + strlen(next_word) + 1));
          w = word_list[i];
          strcat(w, next_word);
          // skip next word for next iteration
          i++;
        }
      }
      simple_command_add_redirection(sc, w);
      continue;
    }

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
        simple_command_add_arg(sc, w);
        // in order to put argv[0] from command name
        //simple_command_set_args_from_string(sc, "");
        s = CMD_ARGS;
        break;
      case CMD_ARGS:
        simple_command_add_arg(sc, w);
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
