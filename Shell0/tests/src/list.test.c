#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../include/cmocka.h"
#include "../../include/list.h"

static int setup(void **state) {
  list* l = list_create();
  if(l == NULL) {
    return -1;
  }

  *state = l;
  return 0;
}

static int teardown(void **state) {
  list_destroy(*state);
  return 0;
}

static void list_add_item(list* l, int dataValue){
  list_item* i = calloc(1, sizeof(list_item));
  int* d = calloc(1, sizeof(int));
  *d = dataValue;
  i->data = d;
  list_push(l, i);
}

static void test_list_create(void **state) {
    list* l = *state;
    assert_null(l->head);
    assert_null(l->tail);
    assert_null(l->current);
    assert_int_equal(l->size, 0);
}

static void test_list_push(void **state) {
    list* l = *state;

    list_add_item(l, 42);

    assert_non_null(l->head);
    assert_non_null(l->head->data);
    assert_int_equal(l->size, 1);
    assert_int_equal((int)(*(int*)((l->head->data))), 42);

    list_add_item(l, 43);

    assert_non_null(l->head->next);
    assert_int_equal(l->size, 2);
    assert_int_equal((int)(*(int*)((l->head->next->data))), 43);
}

static void test_list_get_next(void **state) {
  list* l = *state;

  list_add_item(l, 42);
  list_add_item(l, 43);

  int first = (int)(*(int*)list_get_next(l)->data);
  int second = (int)(*(int*)list_get_next(l)->data);

  assert_int_equal(first, 42);
  assert_int_equal(second, 43);
}

static void test_list_reinit_iteration(void **state) {
  list* l = *state;

  list_add_item(l, 42);
  list_add_item(l, 43);

  int first = (int)(*(int*)list_get_next(l)->data);
  list_reinit_iteration(l);
  int second = (int)(*(int*)list_get_next(l)->data);

  assert_int_equal(first, 42);
  assert_int_equal(second, 42);
}

static void test_list_destroy(void **state) {
  list* l = list_create();
  list_add_item(l, 42);
  list_add_item(l, 43);
  list_destroy(l);
}

const struct CMUnitTest open_i2c_tests[] = {
  cmocka_unit_test_setup_teardown(test_list_create, setup, teardown),
  cmocka_unit_test_setup_teardown(test_list_push, setup, teardown),
  cmocka_unit_test_setup_teardown(test_list_get_next, setup, teardown),
  cmocka_unit_test_setup_teardown(test_list_reinit_iteration, setup, teardown),
  cmocka_unit_test(test_list_destroy),
};

int main(void)
{
    return cmocka_run_group_tests(open_i2c_tests, NULL, NULL);
}
