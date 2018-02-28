#include <stdlib.h>
#include <stdio.h>

#include "../include/pipeline.h"
#include "../include/utils.h"

pipeline* pipeline_create() {
  pipeline* p = (pipeline*) calloc(1, sizeof(pipeline));
  p->simple_commands = calloc(1, sizeof(list));
  return p;
}

void pipeline_destroy(pipeline* p) {
  int nb_commands = p->simple_commands->size;

  if(nb_commands > 0){
    list_item* current = NULL;
    current = p->simple_commands->head;
    list_item* to_del = NULL;
    while(current != NULL) {
      to_del = current;
      current = current->next;
      simple_command_destroy(to_del->data);
      free(to_del);
    }
  }
  //list_destroy(p->simple_commands);
  free(p->simple_commands);
  free(p);
}

void pipeline_add_simple_command(pipeline* p, simple_command* sc) {
  list_item* li = calloc(1, sizeof(list_item));
  li->data = sc;
  list_push(p->simple_commands, li);
}

simple_command* pipeline_get_next_simple_command(pipeline* p){
  list_item* li = list_get_next(p->simple_commands);
  return li ? ((simple_command*) li->data) : NULL;
}

pipeline* pipeline_from_string(char* str){
  pipeline* p = pipeline_create();
  int str_l = strlen(str);
  char* simple_command_str_buffer = calloc(1, sizeof(char));
  int simple_command_str_buffer_l = 0;
  char c = '\0';

  for(int i = 0; i < str_l; i++){
    c = str[i];
    if(c != '|') {
      simple_command_str_buffer = realloc(simple_command_str_buffer, (simple_command_str_buffer_l + 2) * sizeof(char));
      simple_command_str_buffer[simple_command_str_buffer_l] = c;
      simple_command_str_buffer_l++;
    } else {
      simple_command_str_buffer[simple_command_str_buffer_l] = '\0';
      simple_command_str_buffer_l = 0;
      pipeline_add_simple_command(p, simple_command_from_string(simple_command_str_buffer));
    }
  }

  if(simple_command_str_buffer != NULL){
    pipeline_add_simple_command(p, simple_command_from_string(simple_command_str_buffer));
  }

  free(simple_command_str_buffer);

  return p;
}
