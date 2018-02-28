#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../include/cmocka.h"
#include "../../include/pipeline.h"

static int setup(void **state) {
  pipeline* p = pipeline_create();
  if(p == NULL) {
    return -1;
  }

  *state = p;
  return 0;
}

static int teardown(void **state) {
  pipeline_destroy(*state);
  return 0;
}

static void test_pipeline_create(void **state) {
    pipeline* p = *state;
    assert_int_equal(p->simple_commands->size, 0);
}

static void test_pipeline_add_simple_command(void **state) {
    pipeline* p = *state;
    simple_command* sc1 = simple_command_from_string("ls -ali /home");
    simple_command* sc2 = simple_command_from_string("grep file");
    pipeline_add_simple_command(p, sc1);
    pipeline_add_simple_command(p, sc2);
    assert_non_null(((simple_command*) sc1)->name);
    assert_string_equal("ls", ((simple_command*) p->simple_commands->head->data)->name);
    assert_string_equal("grep", ((simple_command*) p->simple_commands->head->next->data)->name);
}

static void test_pipeline_get_next_simple_command(void **state){
  pipeline* p = *state;
  simple_command* sc1 = simple_command_from_string("ls -ali /home");
  simple_command* sc2 = simple_command_from_string("grep file");
  pipeline_add_simple_command(p, sc1);
  pipeline_add_simple_command(p, sc2);
  simple_command* sc_1 = pipeline_get_next_simple_command(p);
  simple_command* sc_2 = pipeline_get_next_simple_command(p);
  assert_true(sc1 == sc_1);
  assert_true(sc2 == sc_2);
}

static void test_pipeline_from_string(void **state){
  pipeline* p = pipeline_from_string("ls -ali | grep file");
  assert_string_equal("ls", ((simple_command*) p->simple_commands->head->data)->name);
  assert_string_equal("grep", ((simple_command*) p->simple_commands->head->next->data)->name);
  pipeline_destroy(p);
}

static void test_pipeline_destroy(void **state) {
  pipeline* p = *state;
  pipeline_destroy(p);
}

const struct CMUnitTest tests[] = {
  cmocka_unit_test_setup_teardown(test_pipeline_create, setup, teardown),
  cmocka_unit_test_setup_teardown(test_pipeline_add_simple_command, setup, teardown),
  cmocka_unit_test_setup_teardown(test_pipeline_get_next_simple_command, setup, teardown),
  cmocka_unit_test(test_pipeline_from_string),
  cmocka_unit_test_setup(test_pipeline_destroy, setup)
};

int main(void)
{
    return cmocka_run_group_tests(tests, NULL, NULL);
}
