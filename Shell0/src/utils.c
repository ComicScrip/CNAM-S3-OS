#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "../include/utils.h"

void free_if_needed(void* to_free)
{
    if (to_free != NULL) free(to_free);
}

void handle_error(int check, char * msg) {
    if(check) {
        perror(msg);
        exit(EXIT_FAILURE);
    }
}

int contains(char* haystack, char* needle){
  const int haystack_length = strlen(haystack);
  const int needle_length = strlen(needle);

  if(needle_length == 0 || needle_length == 0 || needle_length > haystack_length) {
    return 0;
  }

  for(int i = 0, needle_idx = 0; i<haystack_length; i++){
    if(haystack[i] == needle[needle_idx]) needle_idx++;
    if(needle_idx == (needle_length)) return 1;
  }

  return 0;
}

char** words(char* str, int* nb_words) {
  char** ret = NULL;
  const int l = strlen(str);
  int word_idx = -1;
  int in_word_idx = 0;
  char c = '\0';
  char in_word = 0;
  int in_quotes = 0;
  int escape_next_car = 0;

  for(int i = 0; i < l; i++){
    c = str[i];

    if(c == '\\') escape_next_car = 1;
    else escape_next_car = 0;

    if(!escape_next_car && (c == '\'' || c == '\"')) in_quotes = !in_quotes;

    if(in_quotes || (c != ' ' && c != '\t' && c != '\n')) { // non-blank character
      in_word = 1;
      if(in_word_idx == 0){ // create a new entry in returned array
        word_idx++;
        ret = realloc(ret, (word_idx + 1) * sizeof(char*));
        ret[word_idx] = NULL;
      }
      ret[word_idx] = realloc(ret[word_idx], (in_word_idx + 2) * sizeof(char*));
      ret[word_idx][in_word_idx] = c;
      in_word_idx++;
    } else { // blank character
      if(in_word) { // if we were in a word and not anymore, update the next word index.
        in_word = 0;
        // terminate current word
        ret[word_idx][in_word_idx] = '\0';
        in_word_idx = 0;
      }
    }
  }

  // terminate last word if needed
  if(in_word_idx != 0){
    ret[word_idx][in_word_idx] = '\0';
  }

  if(nb_words){
    *nb_words = word_idx + 1;
  }

  return ret;
}

int index_of(char* haystack, char* needle, int occurence_number){
  const int haystack_length = strlen(haystack);
  const int needle_length = strlen(needle);
  int ret = -1;

  if(needle_length == 0 || needle_length == 0 || needle_length > haystack_length) {
    return 0;
  }

  if(occurence_number < 1) return ret;

  for(int i = 0, needle_idx = 0; i<haystack_length; i++){
    if(haystack[i] == needle[needle_idx]) needle_idx++;
    if(occurence_number == 1 && needle_idx == 1) ret = i;
    if(needle_idx == needle_length) {
      occurence_number--;
      needle_idx = 0;
    }
    if(occurence_number == 0) return ret;
  }

  return ret;
}


char* strcat_dyn(char* str, char* to_append){
  char* ret = realloc(str, strlen(str) + strlen(to_append) + 1);
  strncat(str, to_append,to_append == '\0' ? 1 : strlen(to_append));
  return ret;
}

char* append(char* str, char to_append){
  int str_l = strlen(str);
  char* ret = realloc(str, sizeof(char) * (str_l + 2));
  ret[str_l] = to_append;
  ret[str_l + 1] = '\0';
  return ret;
}

char* strip_quotes(char* str){
  int str_l = strlen(str);
  char * ret = calloc(1, sizeof(char));
  int ret_idx = 0;
  int escape_next_car = 0;
  for(int i = 0; i < str_l; i++) {
      if((str[i] == '\'' || str[i] == '\"')){
        if(escape_next_car){
          ret = realloc(ret, sizeof(char) * (ret_idx + 2));
          ret[ret_idx++] = str[i];
        }
      } else if(str[i] != '\\'){
        ret = realloc(ret, sizeof(char) * (ret_idx + 2));
        ret[ret_idx++] = str[i];
      }

      if(str[i] == '\\') escape_next_car = 1;
      else escape_next_car = 0;
  }
  ret[ret_idx] = '\0';
  free(str);
  return ret;
}

char** split(char* str, char c, int* nb_parts, int parts_needed) {
  int str_l = strlen(str);
  char** ret = calloc(1, sizeof(char*));
  int parts_l = 0;
  int part_idx = 0;
  int do_split = 1;
  int i;

  for(i=0; i < str_l; i++){
    ret = realloc(ret, sizeof(char*) * (parts_l + 1));
    ret[parts_l] = realloc(ret[parts_l], (part_idx + 1) * sizeof(char));
    if(str[i] == c && do_split) {
      ret[parts_l][part_idx] = '\0';
      part_idx = 0;
      if(i != (str_l - 1)) parts_l++;
      if(parts_l == (parts_needed - 1)) do_split = 0;
    } else {
      ret[parts_l] = realloc(ret[parts_l], (part_idx + 1)*sizeof(char));
      ret[parts_l][part_idx] = str[i];
      part_idx++;
    }
  }

  if(part_idx != 0){
    ret[parts_l][part_idx] = '\0';
  }

  if(nb_parts != NULL) *nb_parts = parts_l + 1;

  return ret;
}
