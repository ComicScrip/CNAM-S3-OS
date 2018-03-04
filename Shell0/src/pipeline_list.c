#include <stdlib.h>
#include <stdio.h>

#include "../include/pipeline_list.h"
#include "../include/utils.h"

pipeline_list* pipeline_list_create() {
  pipeline_list* pl = (pipeline_list*) calloc(1, sizeof(pipeline_list));
  pl->pipelines = calloc(1, sizeof(list));
  return pl;
}

void pipeline_list_destroy(pipeline_list* pl) {
  int nb_commands = pl->pipelines->size;

  if(nb_commands > 0){
    list_item* current = NULL;
    current = pl->pipelines->head;
    list_item* to_del = NULL;
    while(current != NULL) {
      to_del = current;
      current = current->next;
      pipeline_destroy(to_del->data);
      free(to_del);
    }
  }
  //list_destroy(pl->pipelines);
  free(pl->pipelines);
  free(pl);
}

void pipeline_list_add_pipeline(pipeline_list* pl, pipeline* sc) {
  list_item* li = calloc(1, sizeof(list_item));
  li->data = sc;
  list_push(pl->pipelines, li);
}

pipeline* pipeline_list_get_next_pipeline(pipeline_list* pl){
  list_item* li = list_get_next(pl->pipelines);
  return li ? ((pipeline*) li->data) : NULL;
}

pipeline_list* pipeline_list_from_string(char* str){
  pipeline_list* pl = pipeline_list_create();
  int str_l = strlen(str);
  char* pipeline_str_buffer = calloc(1, sizeof(char));
  char c = '\0';
  int in_quotes = 0;
  char pipe_encontered = 0;
  char ampersand_encontered = 0;
  pipeline* p = NULL;
  int escape_next_car = 0;

  for(int i = 0; i < str_l; i++){
    c = str[i];

    if(c == '\"' || c == '\'') {
      if(!escape_next_car){
        in_quotes = !in_quotes;
      }
      pipeline_str_buffer = append(pipeline_str_buffer, c);
    }
    else {
      if(c == '\\') escape_next_car = 1;
      else escape_next_car = 0;
      if(in_quotes){
        pipeline_str_buffer = append(pipeline_str_buffer, c);
        continue;
      }

      if(c == '|') {
        if(pipe_encontered) {
          pipeline_str_buffer[strlen(pipeline_str_buffer) - 1] = '\0'; // cancel previous |
          p = pipeline_from_string(pipeline_str_buffer);
          p->terminating_token = OR;
          pipeline_list_add_pipeline(pl, p);
          pipeline_str_buffer[0] = '\0';
          pipe_encontered = 0;
          continue;
        }
        pipe_encontered = 1;
      } else { pipe_encontered = 0; }

      if (c == '&') {
        if(ampersand_encontered) {
          pipeline_str_buffer[strlen(pipeline_str_buffer) - 1] = '\0'; // canel previous &
          p = pipeline_from_string(pipeline_str_buffer);
          p->terminating_token = AND;
          pipeline_list_add_pipeline(pl, p);
          pipeline_str_buffer[0] = '\0';
          ampersand_encontered = 0;
          continue;
        }
        ampersand_encontered = 1;
      } else if (ampersand_encontered){
        ampersand_encontered = 0;
        pipeline_str_buffer[strlen(pipeline_str_buffer) - 1] = '\0'; // canel previous &
        p = pipeline_from_string(pipeline_str_buffer);
        p->terminating_token = ASYNC;
        pipeline_list_add_pipeline(pl, p);
        pipeline_str_buffer[0] = '\0';
        ampersand_encontered = 0;
        continue;
      }

      if (c == ';' || c == '\n') {
        p = pipeline_from_string(pipeline_str_buffer);
        p->terminating_token = SEPARATOR;
        pipeline_list_add_pipeline(pl, p);
        pipeline_str_buffer[0] = '\0';
        continue;
      }
      pipeline_str_buffer = append(pipeline_str_buffer, c);
    }
  }

  if(strlen(pipeline_str_buffer)) {
    int async = 0;
    if(pipeline_str_buffer[strlen(pipeline_str_buffer) -1] == '&') {
      async = 1;
      pipeline_str_buffer[strlen(pipeline_str_buffer) -1] = '\0'; // canel previous &
    }
    p = pipeline_from_string(pipeline_str_buffer);
    if(async) { p->terminating_token = ASYNC; }
    pipeline_list_add_pipeline(pl, p);
  }

  free(pipeline_str_buffer);

  return pl;
}
