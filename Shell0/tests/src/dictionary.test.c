#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../include/cmocka.h"
#include "../../include/dictionary.h"

static void test_dictionnary(void **state){
  dictionary* d = dictionary_create();
  assert_int_equal(d->size, 0);
  dictionary_set(d, "key1", "val1");
  assert_int_equal(d->size, 1);
  assert_string_equal("val1", dictionary_get(d, "key1"));
  dictionary_set(d, "key1", "val2");
  assert_int_equal(d->size, 1);
  assert_string_equal("val2", dictionary_get(d, "key1"));
  assert_null(dictionary_get(d, "key3"));
  dictionary_destroy(d);
}

const struct CMUnitTest open_i2c_tests[] = {
  cmocka_unit_test(test_dictionnary),
};

int main(void)
{
    return cmocka_run_group_tests(open_i2c_tests, NULL, NULL);
}
