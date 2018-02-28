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

  for(int i = 0; i < l; i++){
    c = str[i];
    if(c != ' ' && c != '\t' && c != '\n') { // non-blank character
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