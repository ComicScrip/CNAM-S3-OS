#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../include/cmocka.h"
#include "../../include/utils.h"

static void test_utils_contains(void **state){
  assert_true(contains("abc", "a"));
  assert_true(contains("abc", "b"));
  assert_true(contains("abc", "c"));
  assert_true(contains("abc", "ab"));
  assert_true(contains("abc", "bc"));
  assert_false(contains("abc", "hhhhhhh"));
  assert_false(contains("abc", "h"));
  assert_false(contains("", "c"));
  assert_false(contains("c", ""));
  assert_false(contains("", ""));
}

static void test_utils_words(void ** state){
  char* str = "   ls -ali /home \"test\" \n test2     \t 'test3'   ";
  int nb_words = 0;
  char** w = words(str, &nb_words);

  assert_null(words("", NULL));
  assert_string_equal(w[0], "ls"); free(w[0]);
  assert_string_equal(w[1], "-ali"); free(w[1]);
  assert_string_equal(w[2], "/home"); free(w[2]);
  assert_string_equal(w[3], "\"test\""); free(w[3]);
  assert_string_equal(w[4], "test2"); free(w[4]);
  assert_string_equal(w[5], "'test3'"); free(w[5]);
  assert_int_equal(nb_words, 6);

  free(w);


  char* str2 = "ls \" my folder with spaces\"   ' t e st'  \"\\\"quotes\\\"\"";
  int nb_words2 = 0;
  char** w2 = words(str2, &nb_words2);

  assert_string_equal(w2[0], "ls"); free(w2[0]);
  assert_string_equal(w2[1], "\" my folder with spaces\""); free(w2[1]);
  assert_string_equal(w2[2], "' t e st'"); free(w2[2]);
  assert_string_equal(w2[3], "\"\\\"quotes\\\"\""); free(w2[3]);

  assert_int_equal(nb_words, 6);
  free(w2);
}

static void test_utils_index_of(void** state) {
  assert_int_equal(index_of("Hello world !", "lo", 1), 3);
  assert_int_equal(index_of("Hello world !", "o", 2), 7);
  assert_int_equal(index_of("Hello world !", "@", 1), -1);
}

static void test_utils_strip_quotes(void** state) {
  char* str = calloc(1, 20 * sizeof(char));
  strcpy(str, "\"test\"");

  str = strip_quotes(str);
  assert_string_equal(str, "test");
  strcpy(str, "\"\\\"test\\\""); // '"\"test\""'
  str = strip_quotes(str);
  assert_string_equal(str, "\"test\"");
  strcpy(str, "''test''");
  str = strip_quotes(str);
  assert_string_equal(str, "test");

  free(str);
}

const struct CMUnitTest open_i2c_tests[] = {
  cmocka_unit_test(test_utils_contains),
  cmocka_unit_test(test_utils_words),
  cmocka_unit_test(test_utils_index_of),
  cmocka_unit_test(test_utils_strip_quotes),
};

int main(void)
{
    return cmocka_run_group_tests(open_i2c_tests, NULL, NULL);
}
